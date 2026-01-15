#include <iostream>
#include <unistd.h>
#include <sys/mman.h>

void wait_stage(const char* stage)
{
    std::cout << stage << std::endl;
    std::cout << "PID процесса: " << getpid() << std::endl;
    std::cout << "Нажмите Enter для продолжения..." << std::endl;
    std::cin.get();
}

int main(int argc, char* argv[])
{
    /* Размер страницы */
    size_t pageSize = sysconf(_SC_PAGESIZE);

    size_t totalPages = 256;
    size_t halfPages  = 128;

    size_t totalSize = totalPages * pageSize;
    size_t halfSize  = halfPages  * pageSize;

    /* Этап 1. Резервирование виртуальной памяти */
    void* base = mmap(
        nullptr,
        totalSize,
        PROT_NONE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (base == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    std::cout << "Базовый адрес области: " << base << std::endl;
    wait_stage("Этап 1: зарезервировано 256 страниц (PROT_NONE)");

    /* Этап 2. Разрешение RW для второй половины */
    void* secondHalf = static_cast<char*>(base) + halfSize;

    if (mprotect(secondHalf, halfSize, PROT_READ | PROT_WRITE) != 0)
    {
        perror("mprotect RW");
        return 1;
    }

    wait_stage("Этап 2: для второй половины установлены PROT_READ | PROT_WRITE");

    /* Этап 3. Заполнение памяти */
    int* data = static_cast<int*>(secondHalf);
    size_t count = halfSize / sizeof(int);

    for (size_t i = 0; i < count; ++i)
        data[i] = static_cast<int>(i);

    wait_stage("Этап 3: выполнена запись данных (выделена физическая память)");

    /* Этап 4. Только чтение */
    if (mprotect(secondHalf, halfSize, PROT_READ) != 0)
    {
        perror("mprotect R");
        return 1;
    }

    wait_stage("Этап 4: страницы переведены в режим PROT_READ");

    /* Этап 5. Освобождение 128 страниц */
    if (munmap(secondHalf, halfSize) != 0)
    {
        perror("munmap second half");
        return 1;
    }

    wait_stage("Этап 5: освобождены 128 страниц с массивом");

    /* Этап 6. Освобождение оставшейся памяти */
    if (munmap(base, halfSize) != 0)
    {
        perror("munmap first half");
        return 1;
    }

    wait_stage("Этап 6: освобождены оставшиеся виртуальные страницы");

    return 0;
}
