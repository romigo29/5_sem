#include "pch.h"
#include "HT.h"
#include <cstring>
#include <cstdlib>
#include <cstdarg>

using namespace std;

    enum SlotStatus : unsigned char {
        SLOT_EMPTY = 0,
        SLOT_OCCUPIED = 1,
        SLOT_DELETED = 2
    };

    static unsigned long hash_bytes(const void* key, int keylen) {
        const unsigned char* ptr = static_cast<const unsigned char*>(key);
        unsigned long h = 5381;
        for (int i = 0; i < keylen; ++i) {
            h = ((h << 5) + h) + ptr[i];
        }
        return h;
    }

    static size_t slot_size_for(const HTHANDLE* ht) {
        return sizeof(unsigned char) + sizeof(int) + sizeof(int)
            + (size_t)ht->MaxKeyLength + (size_t)ht->MaxPayloadLength;
    }

    static size_t compute_file_size(const HTHANDLE* ht) {
        return sizeof(HTHANDLE) + (size_t)ht->Capacity * slot_size_for(ht);
    }

    static unsigned char* slot_ptr(const HTHANDLE* ht, int index) {
        char* base = static_cast<char*>(ht->Addr);
        return reinterpret_cast<unsigned char*>(
            base + sizeof(HTHANDLE) + (size_t)index * slot_size_for(ht)
            );
    }

    static void read_slot(const HTHANDLE* ht, int index,
        unsigned char& status, int& keylen, int& payloadlen,
        const char*& keybuf, const char*& payloadbuf) {
        unsigned char* p = slot_ptr(ht, index);
        status = p[0];
        char* q = reinterpret_cast<char*>(p + 1);
        memcpy(&keylen, q, sizeof(int)); q += sizeof(int);
        memcpy(&payloadlen, q, sizeof(int)); q += sizeof(int);
        keybuf = q;
        payloadbuf = q + ht->MaxKeyLength;
    }

    static void write_slot_header(const HTHANDLE* ht, int index,
        unsigned char status, int keylen, int payloadlen) {
        unsigned char* p = slot_ptr(ht, index);
        p[0] = status;
        char* q = reinterpret_cast<char*>(p + 1);
        memcpy(q, &keylen, sizeof(int)); q += sizeof(int);
        memcpy(q, &payloadlen, sizeof(int)); q += sizeof(int);
    }

    static void write_slot_buffers(const HTHANDLE* ht, int index,
        const void* key, int keylen,
        const void* payload, int payloadlen) {
        unsigned char* p = slot_ptr(ht, index);
        char* q = reinterpret_cast<char*>(p + 1 + sizeof(int) + sizeof(int));
        if (keylen > 0 && key) memcpy(q, key, (size_t)keylen);
        if ((size_t)keylen < (size_t)ht->MaxKeyLength)
            memset(q + keylen, 0, (size_t)ht->MaxKeyLength - keylen);

        char* payloadbuf = q + ht->MaxKeyLength;
        if (payloadlen > 0 && payload) memcpy(payloadbuf, payload, (size_t)payloadlen);
        if ((size_t)payloadlen < (size_t)ht->MaxPayloadLength)
            memset(payloadbuf + payloadlen, 0,
                (size_t)ht->MaxPayloadLength - payloadlen);
    }


    static std::string make_mutex_name(const char* fileName)
    {
        std::string name = "HT_MUTEX_";
        for (const char* p = fileName; p && *p; ++p) {
            char c = *p;
            if (c == '\\' || c == '/' || c == ':' || c == ' ')
                name.push_back('_');
            else
                name.push_back(c);
        }
        return name;
    }

    static HANDLE CreateStorageMutex(const char* fileName)
    {
        std::string mtxName = make_mutex_name(fileName);
        HANDLE h = CreateMutexA(NULL, FALSE, mtxName.c_str());
        return h;
    }

    static DWORD WINAPI SnapshotThreadProc(LPVOID param)
    {
        HTHANDLE* ht = (HTHANDLE*)param;
        if (!ht) return 0;

        int interval = ht->SecSnapshotInterval;
        if (interval <= 0) interval = 1;

        while (InterlockedCompareExchange(&ht->isRunning, 1, 1) == 1)
        {
            Sleep((DWORD)interval * 1000);
            if (InterlockedCompareExchange(&ht->isRunning, 1, 1) == 0) break;
            Snap(ht);
        }
        return 0;
    }

    HTHANDLE* Create(
        int Capacity,
        int SecSnapshotInterval,
        int MaxKeyLength,
        int MaxPayloadLength,
        const char FileName[SIZE]
    )
    {

        HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval,
            MaxKeyLength, MaxPayloadLength, FileName);

        ht->File = CreateFileA(
            FileName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (ht->File == INVALID_HANDLE_VALUE) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать файл", SIZE);
            delete ht;
            return nullptr;
        }
        size_t fileSize = compute_file_size(ht);
        LARGE_INTEGER li;  li.QuadPart = (LONGLONG)fileSize;
        if (!SetFilePointerEx(ht->File, li, NULL, FILE_BEGIN) ||
            !SetEndOfFile(ht->File))
        {
            strncpy_s(ht->LastErrorMessage, "Не удалось установить размер файла", SIZE);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, (DWORD)fileSize, NULL);
        if (!ht->FileMapping) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать проекцию файла", SIZE);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, (SIZE_T)fileSize);
        if (!ht->Addr) {
            strncpy_s(ht->LastErrorMessage, "Не удалось отобразить файл в память", SIZE);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        HTHANDLE tempHdr = *ht;
        tempHdr.File = INVALID_HANDLE_VALUE;
        tempHdr.FileMapping = NULL;
        tempHdr.Addr = NULL;

        memcpy(ht->Addr, &tempHdr, sizeof(HTHANDLE));

        size_t slotSize = slot_size_for(ht);
        char* slotsStart = static_cast<char*>(ht->Addr) + sizeof(HTHANDLE);
        memset(slotsStart, 0, (size_t)ht->Capacity * slotSize);

        ht->lastsnaptime = time(nullptr);
        tempHdr = *ht;
        tempHdr.File = INVALID_HANDLE_VALUE;
        tempHdr.FileMapping = NULL;
        tempHdr.Addr = NULL;

        memcpy(ht->Addr, &tempHdr, sizeof(HTHANDLE));


        ht->hMutex = CreateStorageMutex(ht->FileName);
        if (ht->hMutex == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать mutex", SIZE);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        InterlockedExchange(&ht->isRunning, 1);
        ht->snapshotThread = CreateThread(NULL, 0, SnapshotThreadProc, ht, 0, NULL);
        if (ht->snapshotThread == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось запустить snapshot-поток", SIZE);
            CloseHandle(ht->hMutex);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }
        return ht;
    }

    HTHANDLE* Open(const char FileName[SIZE])
    {
        HTHANDLE* ht = new HTHANDLE();
        strncpy_s(ht->FileName, FileName, SIZE);

        ht->File = CreateFileA(
            FileName,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL

        );
        if (ht->File == INVALID_HANDLE_VALUE) {
            strncpy_s(ht->LastErrorMessage, "Не удалось открыть файл", SIZE);
            delete ht;
            return nullptr;
        }

        LARGE_INTEGER liSize;
        if (!GetFileSizeEx(ht->File, &liSize)) {
            strncpy_s(ht->LastErrorMessage, "Не удалось получить размер файла", SIZE);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }
        SIZE_T fileSize = (SIZE_T)liSize.QuadPart;

        ht->FileMapping = CreateFileMappingA(ht->File, NULL, PAGE_READWRITE, 0, (DWORD)fileSize, NULL);
        if (!ht->FileMapping) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать проекцию файла", SIZE);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }
        ht->Addr = MapViewOfFile(ht->FileMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
        if (!ht->Addr) {
            strncpy_s(ht->LastErrorMessage, "Не удалось отобразить файл в память", SIZE);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        HTHANDLE temp;
        memcpy(&temp, ht->Addr, sizeof(HTHANDLE));

        HANDLE fileHandle = ht->File;
        HANDLE mapHandle = ht->FileMapping;
        LPVOID addr = ht->Addr;

        *ht = temp;
        ht->File = fileHandle;
        ht->FileMapping = mapHandle;
        ht->Addr = addr;

        ht->hMutex = CreateStorageMutex(ht->FileName);
        if (ht->hMutex == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать mutex", SIZE);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        InterlockedExchange(&ht->isRunning, 1);
        ht->snapshotThread = CreateThread(NULL, 0, SnapshotThreadProc, ht, 0, NULL);
        if (ht->snapshotThread == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось запустить snapshot-поток", SIZE);
            CloseHandle(ht->hMutex);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        return ht;
    }

    BOOL Snap(const HTHANDLE* hthandle)
    {
        if (!hthandle || !hthandle->Addr) return FALSE;

        BOOL haveMutex = FALSE;
        if (hthandle->hMutex) {
            DWORD w = WaitForSingleObject(hthandle->hMutex, INFINITE);
            haveMutex = (w == WAIT_OBJECT_0 || w == WAIT_ABANDONED);
        }

        HTHANDLE* ht = const_cast<HTHANDLE*>(hthandle);
        ht->lastsnaptime = time(nullptr);

        HTHANDLE tmp = *ht;
        tmp.File = INVALID_HANDLE_VALUE;
        tmp.FileMapping = NULL;
        tmp.Addr = NULL;
        memcpy(ht->Addr, &tmp, sizeof(HTHANDLE));

        if (!FlushViewOfFile(ht->Addr, 0)) {
            strncpy_s(ht->LastErrorMessage, "FlushViewOfFile failed", SIZE);
            if (haveMutex) ReleaseMutex(hthandle->hMutex);
            return FALSE;
        }
        if (!FlushFileBuffers(ht->File)) {
            strncpy_s(ht->LastErrorMessage, "FlushFileBuffers failed", SIZE);
            if (haveMutex) ReleaseMutex(hthandle->hMutex);
            return FALSE;
        }
        if (haveMutex) ReleaseMutex(hthandle->hMutex);
        return TRUE;
    }

    BOOL Close(HTHANDLE* hthandle)
    {
        if (hthandle == nullptr) return FALSE;

        BOOL result = TRUE;

        if (!Snap(hthandle)) result = FALSE;
        if (hthandle->snapshotThread != NULL) {
            InterlockedExchange(&const_cast<HTHANDLE*>(hthandle)->isRunning, 0);
            WaitForSingleObject(hthandle->snapshotThread, INFINITE);
            CloseHandle(hthandle->snapshotThread);
            const_cast<HTHANDLE*>(hthandle)->snapshotThread = NULL;
        }

        if (hthandle->Addr != nullptr) UnmapViewOfFile(hthandle->Addr);
        if (hthandle->FileMapping != NULL) CloseHandle(hthandle->FileMapping);
        if (hthandle->File != INVALID_HANDLE_VALUE) CloseHandle(hthandle->File);

        if (hthandle->hMutex != NULL) {
            CloseHandle(hthandle->hMutex);
            const_cast<HTHANDLE*>(hthandle)->hMutex = NULL;
        }

        delete hthandle;

        return result;
    }

    BOOL Insert(const HTHANDLE* hthandle, const Element* element)
    {
        if (!hthandle || !element || !element->key) return FALSE;

        if (element->keylength <= 0 || element->keylength > hthandle->MaxKeyLength) {
            strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                "Неверная длина ключа", SIZE);
            return FALSE;
        }
        if (element->payloadlength < 0 || element->payloadlength > hthandle->MaxPayloadLength) {
            strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                "Неверная длина payload", SIZE);
            return FALSE;
        }

        if (hthandle->hMutex) WaitForSingleObject(hthandle->hMutex, INFINITE);
        unsigned long h = hash_bytes(element->key, element->keylength);
        int idxStart = (int)(h % (unsigned long)hthandle->Capacity);
        int capacity = hthandle->Capacity;

        int freeIndex = -1;

        for (int i = 0; i < capacity; ++i) {
            int idx = (idxStart + i) % capacity;
            unsigned char status; int keylen, payloadlen;
            const char* keybuf; const char* payloadbuf;
            read_slot(hthandle, idx, status, keylen, payloadlen, keybuf, payloadbuf);

            if (status == SLOT_DELETED) {
                if (freeIndex == -1) freeIndex = idx;
                continue;
            }
            if (status == SLOT_EMPTY) {
                if (freeIndex == -1) freeIndex = idx;
                break;
            }
            if (status == SLOT_OCCUPIED) {
                if (keylen == element->keylength &&
                    memcmp(keybuf, element->key, (size_t)keylen) == 0)
                {
                    strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                        "Ключ уже существует", SIZE);
                    if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
                    return FALSE;
                }
            }
        }

        if (freeIndex == -1) {
            strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                "Таблица заполнена", SIZE);
            if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
            return FALSE;
        }
        write_slot_header(hthandle, freeIndex,
            SLOT_OCCUPIED, element->keylength, element->payloadlength);
        write_slot_buffers(hthandle, freeIndex,
            element->key, element->keylength,
            element->payload, element->payloadlength);

        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->lastsnaptime) >= hthandle->SecSnapshotInterval)
            Snap(hthandle);

        if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);

        return TRUE;
    }

    BOOL Delete(const HTHANDLE* hthandle, const Element* element)
    {
        if (!hthandle || !element || !element->key) return FALSE;


        if (hthandle->hMutex) WaitForSingleObject(hthandle->hMutex, INFINITE);

        unsigned long h = hash_bytes(element->key, element->keylength);
        int idxStart = (int)(h % (unsigned long)hthandle->Capacity);
        int capacity = hthandle->Capacity;

        for (int i = 0; i < capacity; ++i) {
            int idx = (idxStart + i) % capacity;
            unsigned char status; int keylen, payloadlen;
            const char* keybuf; const char* payloadbuf;
            read_slot(hthandle, idx, status, keylen, payloadlen, keybuf, payloadbuf);

            if (status == SLOT_EMPTY) {
                strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                    "Ключ не найден", SIZE);
                if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
                return FALSE;
            }
            if (status == SLOT_OCCUPIED) {
                if (keylen == element->keylength &&
                    memcmp(keybuf, element->key, (size_t)keylen) == 0)
                {
                    write_slot_header(hthandle, idx, SLOT_DELETED, 0, 0);
                    time_t currentTime = time(nullptr);
                    if (difftime(currentTime, hthandle->lastsnaptime) >= hthandle->SecSnapshotInterval)
                        Snap(hthandle);
                    if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
                    return TRUE;
                }
            }
        }

        strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
            "Ключ не найден", SIZE);
        if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
        return FALSE;
    }

    Element* Get(const HTHANDLE* hthandle, const Element* element)
    {
        if (!hthandle || !element || !element->key) return nullptr;

        unsigned long h = hash_bytes(element->key, element->keylength);
        int idxStart = (int)(h % (unsigned long)hthandle->Capacity);
        int capacity = hthandle->Capacity;

        for (int i = 0; i < capacity; ++i) {
            int idx = (idxStart + i) % capacity;
            unsigned char status; int keylen, payloadlen;
            const char* keybuf; const char* payloadbuf;
            read_slot(hthandle, idx, status, keylen, payloadlen, keybuf, payloadbuf);

            if (status == SLOT_EMPTY) return nullptr;

            if (status == SLOT_OCCUPIED) {
                if (keylen == element->keylength &&
                    memcmp(keybuf, element->key, (size_t)keylen) == 0)
                {
                    Element* res = new Element();
                    char* kcopy = new char[keylen];
                    memcpy(kcopy, keybuf, (size_t)keylen);
                    char* pcopy = nullptr;
                    if (payloadlen > 0) {
                        pcopy = new char[payloadlen];
                        memcpy(pcopy, payloadbuf, (size_t)payloadlen);
                    }
                    res->key = kcopy;
                    res->keylength = keylen;
                    res->payload = pcopy;
                    res->payloadlength = payloadlen;
                    return res;
                }
            }
        }
        return nullptr;
    }

    BOOL Update(const HTHANDLE* hthandle, const Element* oldelement,
        const void* newpayload, int newpayloadlength)
    {
        if (!hthandle || !oldelement) return FALSE;

        if (newpayloadlength < 0 || newpayloadlength > hthandle->MaxPayloadLength) {
            strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                "Неверная длина payload", SIZE);
            return FALSE;
        }

        if (hthandle->hMutex) WaitForSingleObject(hthandle->hMutex, INFINITE);

        unsigned long h = hash_bytes(oldelement->key, oldelement->keylength);
        int idxStart = (int)(h % (unsigned long)hthandle->Capacity);
        int capacity = hthandle->Capacity;

        for (int i = 0; i < capacity; ++i) {
            int idx = (idxStart + i) % capacity;
            unsigned char status; int keylen, payloadlen;
            const char* keybuf; const char* payloadbuf;
            read_slot(hthandle, idx, status, keylen, payloadlen, keybuf, payloadbuf);

            if (status == SLOT_EMPTY) {
                strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
                    "Ключ не найден", SIZE);
                if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
                return FALSE;
            }
            if (status == SLOT_OCCUPIED) {
                if (keylen == oldelement->keylength &&
                    memcmp(keybuf, oldelement->key, (size_t)keylen) == 0)
                {
                    unsigned char* p = slot_ptr(hthandle, idx);
                    char* payloadBuf = reinterpret_cast<char*>(
                        p + 1 + sizeof(int) + sizeof(int) + hthandle->MaxKeyLength);
                    if (newpayloadlength > 0 && newpayload)
                        memcpy(payloadBuf, newpayload, (size_t)newpayloadlength);
                    if ((size_t)newpayloadlength < (size_t)hthandle->MaxPayloadLength)
                        memset(payloadBuf + newpayloadlength, 0,
                            (size_t)hthandle->MaxPayloadLength - newpayloadlength);

                    write_slot_header(hthandle, idx, SLOT_OCCUPIED, keylen, newpayloadlength);
                    time_t currentTime = time(nullptr);
                    if (difftime(currentTime, hthandle->lastsnaptime) >= hthandle->SecSnapshotInterval)
                        Snap(hthandle);
                    if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
                    return TRUE;
                }
            }
        }
        strncpy_s(const_cast<HTHANDLE*>(hthandle)->LastErrorMessage,
            "Ключ не найден", SIZE);
        if (hthandle->hMutex) ReleaseMutex(hthandle->hMutex);
        return FALSE;
    }
    char* HT_GetLastError(HTHANDLE* ht)
    {
        if (ht == nullptr) {
            static char tmp[] = "Неверный HTHANDLE";
            return tmp;
        }
        return ht->LastErrorMessage;
    }

    void Print(const Element* element)
    {
        if (element == nullptr) {
            cout << "Элемент равен null" << endl;
            return;
        }

        cout << "Ключ: ";
        if (element->key != nullptr) {
            const char* k = static_cast<const char*>(element->key);
            for (int i = 0; i < element->keylength; ++i) cout << k[i];
        }
        cout << endl;

        cout << "Полезная нагрузка: ";
        if (element->payload != nullptr) {
            const char* p = static_cast<const char*>(element->payload);
            for (int i = 0; i < element->payloadlength; ++i) cout << p[i];
        }
        cout << endl;
    }


