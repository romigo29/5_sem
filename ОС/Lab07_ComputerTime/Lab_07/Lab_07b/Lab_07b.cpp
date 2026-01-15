#include <Windows.h>
#include <iostream>

int main() {
    unsigned long long startTime = GetTickCount64();
    unsigned long long elapsedTime = 0;
    unsigned long long nextReport1 = 5000; // 5 секунд
    unsigned long long nextReport2 = 10000; // 10 секунд
    unsigned long long endTime = 15000; // 15 секунд

    unsigned long long counter = 0;

    while (true) {
        counter++; // подсчет итераций
        elapsedTime = GetTickCount64() - startTime;

        if (elapsedTime >= nextReport1) {
            std::cout << "Iterations after 5 seconds: " << counter << std::endl;
            nextReport1 = UINT64_MAX; // больше не выводим
        }

        if (elapsedTime >= nextReport2) {
            std::cout << "Iterations after 10 seconds: " << counter << std::endl;
            nextReport2 = UINT64_MAX; // больше не выводим
        }

        if (elapsedTime >= endTime) {
            std::cout << "Final iterations after 15 seconds: " << counter << std::endl;
            break;
        }
    }

    return 0;
}
