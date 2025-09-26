#include "HT.h"
#include <cstring>
#include <ctime>

using namespace std;

namespace HT
{
    HTHANDLE* Create(
        int Capacity,                   // Емкость
        int SecSnapshotInterval,        // Интервал создания снимков в секундах
        int MaxKeyLength,               // Максимальная длина ключа
        int MaxPayloadLength,           // Максимальная длина полезной нагрузки
        const char FileName[SIZE])      // Имя файла
    {
        HTHANDLE* ht = new HTHANDLE(Capacity, SecSnapshotInterval, MaxKeyLength, MaxPayloadLength, FileName);

        // Создание файла
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

        // Вычисление общего необходимого размера
        DWORD fileSize = sizeof(HTHANDLE) + (Capacity * (sizeof(Element) + MaxKeyLength + MaxPayloadLength));

        // Установка размера файла
        SetFilePointer(ht->File, fileSize, NULL, FILE_BEGIN);
        SetEndOfFile(ht->File);

        // Создание проекции файла (file mapping)
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

        // Отображение представления файла в память
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

        // Копирование структуры HTHANDLE в отображенную память
        memcpy(ht->Addr, ht, sizeof(HTHANDLE));

        ht->lastsnaptime = time(nullptr); // Установка времени последнего снимка
        return ht;
    }

    HTHANDLE* Open(const char FileName[SIZE]) // Открытие существующей хеш-таблицы
    {
        HTHANDLE* ht = new HTHANDLE();
        strncpy_s(ht->FileName, FileName, SIZE);

        // Открытие файла
        ht->File = CreateFileA(
            FileName,
            GENERIC_READ | GENERIC_WRITE,
            0,
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

        // Получение размера файла
        DWORD fileSize = GetFileSize(ht->File, NULL);

        // Создание проекции файла
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

        // Отображение представления файла в память
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

        // Копирование структуры HTHANDLE из отображенной памяти
        memcpy(ht, ht->Addr, sizeof(HTHANDLE));

        return ht;
    }

    BOOL Snap(const HTHANDLE* hthandle) // Создание снимка (сохранение состояния)
    {
        if (hthandle == nullptr || hthandle->Addr == nullptr) {
            return FALSE;
        }

        // Обновление времени последнего снимка
        HTHANDLE* nonConstHt = const_cast<HTHANDLE*>(hthandle);
        nonConstHt->lastsnaptime = time(nullptr);
        memcpy(nonConstHt->Addr, nonConstHt, sizeof(HTHANDLE));

        // Сброс представления для гарантии записи данных на диск
        if (!FlushViewOfFile(hthandle->Addr, 0)) {
            return FALSE;
        }

        return TRUE;
    }

    BOOL Close(const HTHANDLE* hthandle) // Закрытие хеш-таблицы
    {
        if (hthandle == nullptr) {
            return FALSE;
        }

        BOOL result = TRUE;

        // Выполнение снимка
        if (!Snap(hthandle)) {
            result = FALSE;
        }

        // Освобождение ресурсов
        if (hthandle->Addr != nullptr) {
            UnmapViewOfFile(hthandle->Addr);
        }

        if (hthandle->FileMapping != NULL) {
            CloseHandle(hthandle->FileMapping);
        }

        if (hthandle->File != INVALID_HANDLE_VALUE) {
            CloseHandle(hthandle->File);
        }

        // Удаление объекта HTHANDLE
        delete hthandle;

        return result;
    }

    // --- Общая хэш-функция (djb2) ---
    size_t HashKey(const void* key, int len) {
        size_t h = 5381;
        const unsigned char* p = (const unsigned char*)key;
        for (int i = 0; i < len; i++) {
            h = ((h << 5) + h) + p[i];
        }
        return h;
    }

    // --- Поиск слота в таблице ---
    int FindSlot(const HTHANDLE* hthandle, const void* key, int keylen, bool& found) {
        
        Element* table = reinterpret_cast<Element*>(
            static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE));

        int capacity = hthandle->Capacity;
        size_t hash = HashKey(key, keylen);
        int index = hash % capacity;  //стартовая позиция

        //линейное пробирование
        for (int i = 0; i < capacity; i++) {
            int probeIndex = (index + i) % capacity;
            Element* slot = &table[probeIndex];

            if (slot->key == nullptr) {
                found = false; // пустой слот
                return probeIndex;
            }

            if (slot->keylength == keylen &&
                memcmp(slot->key, key, keylen) == 0) {
                found = true; // нашли элемент
                return probeIndex;
            }
        }

        found = false;
        return -1; // таблица заполнена
    }

    BOOL Insert(HTHANDLE* hthandle, const Element* element) {
        if (!hthandle || !element || !element->key || element->keylength <= 0) {
            std::cerr << "Insert: неверные параметры" << std::endl;
            return FALSE;
        }

        char* dataStart = static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE);
        size_t elementBlockSize = sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        Element* table = reinterpret_cast<Element*>(dataStart);

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

        char* elementDataStart = dataStart + slotIndex * elementBlockSize;
        Element* slot = &table[slotIndex];

        // Копируем ключ и payload в memory-mapped region
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

        // Проверка необходимости snapshot
        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->lastsnaptime) >= hthandle->SecSnapshotInterval) {
            Snap(hthandle);
        }

        return TRUE;
    }


    BOOL Delete(HTHANDLE* hthandle, const Element* element) {
        if (!hthandle || !element || !element->key) return FALSE;

        char* dataStart = static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE);
        size_t elementBlockSize = sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        Element* table = reinterpret_cast<Element*>(dataStart);

        bool found = false;
        int slotIndex = FindSlot(hthandle, element->key, element->keylength, found);
        if (!found || slotIndex == -1) {
            std::cerr << "Delete: ключ не найден" << std::endl;
            return FALSE;
        }

        Element* slot = &table[slotIndex];

        // Помечаем как tombstone
        slot->key = (void*)-1;
        slot->keylength = 0;
        slot->payloadlength = 0;
        slot->payload = nullptr;

        // Snapshot
        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->lastsnaptime) >= hthandle->SecSnapshotInterval) {
            Snap(hthandle);
        }

        return TRUE;
    }


    Element* Get(const HTHANDLE* hthandle, const Element* element) {
        if (hthandle == nullptr || element == nullptr || element->key == nullptr) {
            return nullptr;
        }

        Element* table = reinterpret_cast<Element*>(
            static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE));

        bool found = false;
        int slotIndex = FindSlot(hthandle, element->key, element->keylength, found);

        if (!found || slotIndex == -1) {
            return nullptr;
        }

        Element* slot = &table[slotIndex];
        Element* result = new Element();
        result->key = slot->key;
        result->keylength = slot->keylength;
        result->payload = slot->payload;
        result->payloadlength = slot->payloadlength;

        return result;
    }

    BOOL Update(HTHANDLE* hthandle, const Element* oldelement, const void* newpayload, int newpayloadlength) {
        if (!hthandle || !oldelement || !newpayload || newpayloadlength <= 0) return FALSE;

        char* dataStart = static_cast<char*>(hthandle->Addr) + sizeof(HTHANDLE);
        size_t elementBlockSize = sizeof(Element) + hthandle->MaxKeyLength + hthandle->MaxPayloadLength;
        Element* table = reinterpret_cast<Element*>(dataStart);

        bool found = false;
        int slotIndex = FindSlot(hthandle, oldelement->key, oldelement->keylength, found);
        if (!found || slotIndex == -1) {
            std::cerr << "Update: ключ не найден" << std::endl;
            return FALSE;
        }

        Element* slot = &table[slotIndex];

        // Копируем новый payload в memory-mapped region
        memcpy((void*)slot->payload, newpayload, newpayloadlength);
        slot->payloadlength = newpayloadlength;

        // Snapshot
        time_t currentTime = time(nullptr);
        if (difftime(currentTime, hthandle->lastsnaptime) >= hthandle->SecSnapshotInterval) {
            Snap(hthandle);
        }

        return TRUE;
    }


    char* GetLastError(HTHANDLE* ht) // Получение последнего сообщения об ошибке
    {
        if (ht == nullptr) {
            char buffer[] = "Неверный HTHANDLE";
            return buffer;
        }
        return ht->LastErrorMessage;
    }

    void print(const Element* element) // Вывод элемента
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
