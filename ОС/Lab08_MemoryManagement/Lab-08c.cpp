#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h>

void wait_stage(const char* stage)
{
    std::cout << stage << std::endl;
    std::cout << "PID процесса: " << getpid() << std::endl;
    std::cout << "Нажмите Enter для продолжения..." << std::endl;
    std::cin.get();
}

int main()
{
    const size_t blockCount = 10;          // количество блоков
    const size_t blockSize  = 512 * 1024;  // размер одного блока 512 КиБ
    std::vector<int*> blocks(blockCount, nullptr);

    // Этап 1: выделение блоков
    for (size_t i = 0; i < blockCount; ++i)
    {
        blocks[i] = static_cast<int*>(malloc(blockSize));
        if (!blocks[i])
        {
            std::cerr << "Ошибка выделения памяти для блока " << i << std::endl;
            return 1;
        }

        wait_stage(("Этап 1: выделен блок " + std::to_string(i)).c_str());
    }

    // Этап 2: заполнение блоков массивами
    for (size_t i = 0; i < blockCount; ++i)
    {
        size_t count = blockSize / sizeof(int);
        for (size_t j = 0; j < count; ++j)
        {
            blocks[i][j] = static_cast<int>(j);
        }

        wait_stage(("Этап 2: заполнен блок " + std::to_string(i)).c_str());
    }

    // Этап 3: освобождение блоков
    for (size_t i = 0; i < blockCount; ++i)
    {
        free(blocks[i]);
        blocks[i] = nullptr;

        wait_stage(("Этап 3: освобождён блок " + std::to_string(i)).c_str());
    }

    // Этап 4: уничтожение кучи (не требуется явно для malloc/free в Linux)
    wait_stage("Этап 4: завершение программы, куча освобождена");

    return 0;
}
