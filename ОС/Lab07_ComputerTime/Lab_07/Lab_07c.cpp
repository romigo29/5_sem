#include <iostream>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <atomic>

std::atomic<unsigned long long> counter(0);
std::atomic<int> secondsElapsed(0);

void timerHandler(union sigval) {
    secondsElapsed += 3;
    std::cout << "Iterations after " << secondsElapsed << " seconds: " << counter.load() << std::endl;
}

int main() {
    // Создаем таймер
    timer_t timerid;
    struct sigevent sev{};
    sev.sigev_notify = SIGEV_THREAD;   // вызываем функцию в отдельном потоке
    sev.sigev_notify_function = timerHandler;
    sev.sigev_value.sival_ptr = &timerid;

    if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) == -1) {
        perror("timer_create");
        return 1;
    }

    // Настраиваем периодический таймер каждые 3 секунды
    struct itimerspec its{};
    its.it_value.tv_sec = 3;   // первый сигнал через 3 секунды
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 3; // повторение каждые 3 секунды
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timerid, 0, &its, nullptr) == -1) {
        perror("timer_settime");
        return 1;
    }

    // Засекаем время старта
    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Бесконечный цикл подсчета итераций
    while (true) {
        counter++;

        clock_gettime(CLOCK_MONOTONIC, &current);
        double elapsed = current.tv_sec - start.tv_sec + (current.tv_nsec - start.tv_nsec) / 1e9;

        if (elapsed >= 15.0) {
            std::cout << "Final iterations after 15 seconds: " << counter.load() << std::endl;
            std::cout << "Actual runtime: " << elapsed << " seconds" << std::endl;
            break;
        }
    }

    // Удаляем таймер
    timer_delete(timerid);

    return 0;
}
