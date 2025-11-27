// lab05_linux_simple.cpp
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include <sched.h>
#include <bitset>
#include <cstring>

using namespace std;

// Получаем идентификатор потока (аналог GetCurrentThreadId)
pid_t get_tid() {
    return static_cast<pid_t>(syscall(SYS_gettid));
}

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    pid_t processId = getpid();
    pid_t threadId = get_tid();

    // Получаем класс приоритета (политику планирования)
    int policy = sched_getscheduler(0);
    if (policy == -1) {
        perror("Ошибка получения политики планирования");
        return 1;
    }

    // Получаем приоритет потока
    sched_param param{};
    if (sched_getparam(0, &param) == -1) {
        perror("Ошибка получения приоритета потока");
        return 1;
    }

    // Получаем маску родственности процессоров
    cpu_set_t processMask;
    CPU_ZERO(&processMask);
    if (sched_getaffinity(0, sizeof(processMask), &processMask) == -1) {
        perror("Ошибка получения маски родственности");
        return 1;
    }

    // Подсчитываем количество доступных процессоров
    int availableProcessors = CPU_COUNT(&processMask);

    // Преобразуем маску в двоичный вид
    bitset<64> processAffinityMask;
    long totalCPUs = sysconf(_SC_NPROCESSORS_CONF);
    for (int i = 0; i < totalCPUs; ++i) {
        if (CPU_ISSET(i, &processMask))
            processAffinityMask.set(i);
    }

    // Определяем текущий процессор
    int currentProcessor = sched_getcpu();

    // Уровень любезности (nice)
    int niceValue = getpriority(PRIO_PROCESS, 0);

    // === Вывод информации ===
    cout << "Process ID: " << processId << endl;
    cout << "Thread ID: " << threadId << endl;

    cout << "Scheduling policy: "
        << (policy == SCHED_OTHER ? "SCHED_OTHER" :
            policy == SCHED_FIFO ? "SCHED_FIFO" :
            policy == SCHED_RR ? "SCHED_RR" :
            policy == SCHED_BATCH ? "SCHED_BATCH" :
            policy == SCHED_IDLE ? "SCHED_IDLE" : "UNKNOWN")
        << endl;

    cout << "Thread sched_priority: " << param.sched_priority << endl;
    cout << "Nice value: " << niceValue << endl;

    cout << "Process affinity mask: " << processAffinityMask << endl;
    cout << "Available processors: " << availableProcessors << endl;
    cout << "Current processor: " << currentProcessor << endl;

    return 0;
}
