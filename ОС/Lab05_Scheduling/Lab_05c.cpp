// Lab_05c_Linux.cpp
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/syscall.h>
#include <sched.h>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdlib>

using namespace std;

// Получаем идентификатор потока
pid_t get_tid() {
    return static_cast<pid_t>(syscall(SYS_gettid));
}

// Потоковая функция (аналог Lab_05x)
void* thread_func(void* arg) {
    int niceValue = *(int*)arg;

    // Устанавливаем "любезность" (приоритет) потока
    if (setpriority(PRIO_PROCESS, 0, niceValue) == -1)
        perror("setpriority");

    pid_t pid = getpid();
    pid_t tid = get_tid();

    auto start = chrono::high_resolution_clock::now();

    for (int i = 1; i <= 1'000'000; ++i) {
        if (i % 1000 == 0) {
            int cpu = sched_getcpu();
            int niceNow = getpriority(PRIO_PROCESS, 0);
            cout << "PID: " << pid
                 << " | TID: " << tid
                 << " | iteration: " << i
                 << " | nice: " << niceNow
                 << " | CPU: " << cpu
                 << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "TID " << tid << " finished in " << elapsed.count() << " s" << endl;

    return nullptr;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./Lab_05c_Linux <P1> <P2> <P3>" << endl;
        cerr << "Example: ./Lab_05c_Linux 0xFF 0 0" << endl;
        return 1;
    }

    unsigned long long P1 = strtoull(argv[1], nullptr, 0); // маска процессоров
    int P2 = atoi(argv[2]); // nice первого потока
    int P3 = atoi(argv[3]); // nice второго потока

    cout << "Processor affinity mask: " << argv[1] << endl;
    cout << "Thread 1 nice: " << P2 << endl;
    cout << "Thread 2 nice: " << P3 << endl;

    // Устанавливаем общую маску для процесса
    cpu_set_t mask;
    CPU_ZERO(&mask);
    for (int i = 0; i < 64; ++i) {
        if (P1 & (1ULL << i))
            CPU_SET(i, &mask);
    }

    if (sched_setaffinity(0, sizeof(mask), &mask) == -1) {
        perror("sched_setaffinity");
    }

    // Создаём потоки
    pthread_t t1, t2;

    if (pthread_create(&t1, nullptr, thread_func, &P2) != 0) {
        perror("pthread_create t1");
        return 1;
    }

    if (pthread_create(&t2, nullptr, thread_func, &P3) != 0) {
        perror("pthread_create t2");
        return 1;
    }

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);

    cout << "Both threads finished." << endl;
    return 0;
}
