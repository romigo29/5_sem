#include "HT.h"
#include <cstring>
#include <ctime>

using namespace std;

namespace HT
{
    HTHANDLE* Create(
        int Capacity,                    
        int SecSnapshotInterval,             
        int MaxKeyLength,                  
        int MaxPayloadLength,               
        const char FileName[SIZE])        
    {
        HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);

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

        DWORD fileSize = sizeof(HTHANDLE) + (Capacity * (sizeof(Element) + MaxKeyLength + MaxPayloadLength));

        SetFilePointer(ht->File, fileSize, NULL, FILE_BEGIN);
        SetEndOfFile(ht->File);

        ht->FileMapping = CreateFileMappingA(
            ht->File,
            NULL,
            PAGE_READWRITE,
            0,
            fileSize,
            NULL
        );

        if (ht->FileMapping == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать проекцию файла", SIZE);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        ht->Addr = MapViewOfFile(
            ht->FileMapping,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            fileSize
        );

        if (ht->Addr == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось отобразить представление файла", SIZE);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        memcpy(ht->Addr, ht, sizeof(HTHANDLE));

        ht->Mutex = CreateMutex(NULL, FALSE, NULL);
        if (ht->Mutex == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать мьютекс", SIZE);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        ht->LastSnapTime = time(nullptr);     

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

        DWORD fileSize = GetFileSize(ht->File, NULL);

        ht->FileMapping = CreateFileMappingA(
            ht->File,
            NULL,
            PAGE_READWRITE,
            0,
            fileSize,
            NULL
        );

        if (ht->FileMapping == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать проекцию файла", SIZE);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        ht->Addr = MapViewOfFile(
            ht->FileMapping,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            fileSize
        );

        if (ht->Addr == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось отобразить представление файла", SIZE);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }


        memcpy(ht, ht->Addr, sizeof(HTHANDLE));

        ht->File = INVALID_HANDLE_VALUE;
        ht->FileMapping = NULL;
        ht->Mutex = NULL;
        ht->SnapshotTimer = NULL;
  

        ht->Mutex = CreateMutex(NULL, FALSE, NULL);
        if (ht->Mutex == NULL) {
            strncpy_s(ht->LastErrorMessage, "Не удалось создать мьютекс", SIZE);
            UnmapViewOfFile(ht->Addr);
            CloseHandle(ht->FileMapping);
            CloseHandle(ht->File);
            delete ht;
            return nullptr;
        }

        StartSnapshotAsync(ht);

        return ht;
    }

    BOOL StartSnapshotAsync(HTHANDLE* htHandle)
    {
        htHandle->SnapshotTimer = CreateWaitableTimer(NULL, FALSE, NULL);
        LARGE_INTEGER Li{};
        Li.QuadPart = -(SECOND * htHandle->SecSnapshotInterval);      
        SetWaitableTimer(htHandle->SnapshotTimer, &Li, htHandle->SecSnapshotInterval * 1000, snapAsync, htHandle, FALSE);

        return true;
    }

    void CALLBACK snapAsync(LPVOID prm, DWORD, DWORD)
    {
        HTHANDLE* htHandle = (HTHANDLE*)prm;
        Snap(htHandle);
    }

    BOOL Snap(HTHANDLE* hthandle)     
    {
        WaitForSingleObject(hthandle->Mutex, INFINITE);
        if (!FlushViewOfFile(hthandle->Addr, NULL)) {
            ReleaseMutex(hthandle->Mutex);
            return false;
        }

        hthandle->LastSnapTime = time(NULL);
        ReleaseMutex(hthandle->Mutex);
        return true;
    }

    BOOL Close(HTHANDLE* hthandle)
    {
        if (hthandle == nullptr) return FALSE;

        BOOL result = TRUE;

        if (!Snap(hthandle)) result = FALSE;

        if (hthandle->SnapshotTimer != NULL && hthandle->SnapshotTimer != INVALID_HANDLE_VALUE) {
            CancelWaitableTimer(hthandle->SnapshotTimer);
            CloseHandle(hthandle->SnapshotTimer);
            hthandle->SnapshotTimer = NULL;
        }

        if (hthandle->Mutex != NULL && hthandle->Mutex != INVALID_HANDLE_VALUE) {
            CloseHandle(hthandle->Mutex);
            hthandle->Mutex = NULL;
        }

        if (hthandle->Addr != nullptr) {
            UnmapViewOfFile(hthandle->Addr);
            hthandle->Addr = nullptr;
        }

        if (hthandle->FileMapping != NULL && hthandle->FileMapping != INVALID_HANDLE_VALUE) {
            CloseHandle(hthandle->FileMapping);
            hthandle->FileMapping = NULL;
        }

        if (hthandle->File != INVALID_HANDLE_VALUE) {
            CloseHandle(hthandle->File);
            hthandle->File = INVALID_HANDLE_VALUE;
        }

        delete hthandle;

        return result;
    }



    size_t HashKey(const void* key, int len) {
        size_t h = 5381;
        const unsigned char* p = (const unsigned char*)key;
        for (int i = 0; i < len; i++) {
            h = ((h << 5) + h) + p[i];
        }
        return h;
    }

    int FindSlot(HTHANDLE* hthandle, const void* key, int keylen, bool& found) {
        
        Element* table = reinterpret_cast<Element*>(
            static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE));

        int capacity = hthandle->Capacity;
        size_t hash = HashKey(key, keylen);
        int index = hash % capacity;   

        for (int i = 0; i < capacity; i++) {
            int probeIndex = (index + i) % capacity;
            Element* slot = &table[probeIndex];

            if (slot->key == nullptr || slot->key == (void*)-1) {
                found = false;   
                return probeIndex;
            }

            if (slot->keylength == keylen &&
                memcmp(slot->key, key, keylen) == 0) {
                found = true;   
                return probeIndex;
            }
        }

        found = false;
        return -1;   
    }

    BOOL Insert(HTHANDLE* hthandle, const Element* element) {
        if (!hthandle || !element || !element->key || element->keylength <= 0) {
            cout << "Insert: неверные параметры" << std::endl;
            return FALSE;
        }

        WaitForSingleObject(hthandle->Mutex, INFINITE);  

        bool found = false;
        int slotIndex = FindSlot(hthandle, element->key, element->keylength, found);

        if (found) {
            cout << "Insert: ключ уже существует" << std::endl;
            return FALSE;
        }
        if (slotIndex == -1) {
            cout << "Insert: хранилище переполнено" << std::endl;
            return FALSE;
        }

        char* dataStart = static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE);
        size_t elementBlockSize = sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        char* elementDataStart = dataStart + slotIndex * elementBlockSize;
        
        Element* table = reinterpret_cast<Element*>(dataStart);
        Element* slot = &table[slotIndex];

        slot->key = elementDataStart;
        memcpy((void*)slot->key, element->key, element->keylength);
        slot->keylength = element->keylength;

        slot->payload = elementDataStart + hthandle->MaxKeyLength;
        if (element->payload && element->payloadlength > 0) {
            memcpy((void*)slot->payload, element->payload, element->payloadlength);
            slot->payloadlength = element->payloadlength;
        }
        else {
            slot->payloadlength = 0;
        }

        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->LastSnapTime) >= hthandle->SecSnapshotInterval) {
            Snap(hthandle);
        }

        ReleaseMutex(hthandle->Mutex);

        return TRUE;
    }

    BOOL Delete(HTHANDLE* hthandle, const Element* element) {
        if (!hthandle || !element || !element->key) return FALSE;

        WaitForSingleObject(hthandle->Mutex, INFINITE);

        bool found = false;
        int slotIndex = FindSlot(hthandle, element->key, element->keylength, found);
        if (!found || slotIndex == -1) {
            cout << "Delete: ключ не найден" << endl;
            return FALSE;
        }

        char* dataStart = static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE);
        size_t elementBlockSize = sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        Element* table = reinterpret_cast<Element*>(dataStart);

        Element* slot = &table[slotIndex];

        slot->key = (void*)-1;
        slot->keylength = 0;
        slot->payloadlength = 0;
        slot->payload = nullptr;

        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->LastSnapTime) >= hthandle->SecSnapshotInterval) {
            Snap(hthandle);
        }

        ReleaseMutex(hthandle->Mutex);

        return TRUE;
    }

    Element* Get(HTHANDLE* hthandle, const Element* element) {
        if (hthandle == nullptr || element == nullptr || element->key == nullptr) {
            return nullptr;
        }

        WaitForSingleObject(hthandle->Mutex, INFINITE);

        bool found = false;
        int slotIndex = FindSlot(hthandle, element->key, element->keylength, found);

        if (!found || slotIndex == -1) {
            return nullptr;
        }

        Element* table = reinterpret_cast<Element*>(
            static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE));

        Element* slot = &table[slotIndex];
        Element* result = new Element();
        result->key = slot->key;
        result->keylength = slot->keylength;
        result->payload = slot->payload;
        result->payloadlength = slot->payloadlength;

        ReleaseMutex(hthandle->Mutex);
        return result;
    }

    BOOL Update(HTHANDLE* hthandle, const Element* oldelement, const void* newpayload, int newpayloadlength) {
        if (!hthandle || !oldelement || !newpayload || newpayloadlength <= 0) return FALSE;

        WaitForSingleObject(hthandle->Mutex, INFINITE);
       
        bool found = false;
        int slotIndex = FindSlot(hthandle, oldelement->key, oldelement->keylength, found);
        if (!found || slotIndex == -1) {
            cout << "Update: ключ не найден" << std::endl;
            return FALSE;
        }

        char* dataStart = static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE);
        size_t elementBlockSize = sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        Element* table = reinterpret_cast<Element*>(dataStart);

        Element* slot = &table[slotIndex];

        memcpy((void*)slot->payload, newpayload, newpayloadlength);
        slot->payloadlength = newpayloadlength;

        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->LastSnapTime) >= hthandle->SecSnapshotInterval) {
            Snap(hthandle);
        }

        ReleaseMutex(hthandle->Mutex);
        return TRUE;
    }

    char* GetLastError(HTHANDLE* ht)
    {
        if (ht == nullptr) {
            return (char*)"Неверный HTHANDLE";        
        }
        return ht->LastErrorMessage;
    }

    void print(const Element* element)   
    {
        if (element == nullptr) {
            cout << "Element is null" << endl;
            return;
        }

        cout << "Key: ";
        if (element->key != nullptr) {
            for (int i = 0; i < element->keylength; i++) {
                cout << static_cast<const char*>(element->key)[i];
            }
        }
        cout << endl;

        cout << "Payload: ";
        if (element->payload != nullptr) {
            for (int i = 0; i < element->payloadlength; i++) {
                cout << static_cast<const char*>(element->payload)[i];
            }
        }
        cout << endl;
    }
}
