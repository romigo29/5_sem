#include <Windows.h>
#include <iostream>

int main() {
    // —оздаем ожидательный таймер
    HANDLE hTimer = CreateWaitableTimerW(NULL, FALSE, NULL);
    if (!hTimer) {
        std::cerr << "Failed to create timer!" << std::endl;
        return 1;
    }

    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -30000000LL; // 3 секунды в 100-нс интервалах (отрицательное значение Ч relative time)

    unsigned long long counter = 0;
    int secondsElapsed = 0;

    while (secondsElapsed < 15) {
        // ”станавливаем таймер на 3 секунды
        if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, FALSE)) {
            std::cerr << "Failed to set timer!" << std::endl;
            CloseHandle(hTimer);
            return 1;
        }

        // —читаем итерации, пока таймер не сработает
        while (WaitForSingleObject(hTimer, 0) == WAIT_TIMEOUT) {
            counter++;
        }

        secondsElapsed += 3;
        std::cout << "Iterations after " << secondsElapsed << " seconds: " << counter << std::endl;
    }

    std::cout << "Final iterations after 15 seconds: " << counter << std::endl;

    CloseHandle(hTimer);
    return 0;
}
