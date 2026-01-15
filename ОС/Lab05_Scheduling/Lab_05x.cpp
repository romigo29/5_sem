// lab05_linux_cycle.cpp
#include <iostream>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <ctime>
#include <chrono>
#include <thread>

using namespace std;

// Получение идентификатора потока (TID)
pid_t get_tid() {
    return static_cast<pid_t>(syscall(SYS_gettid));
}

int main() {
    pid_t pid = getpid();       // идентификатор процесса
    pid_t tid = get_tid();      // идентификатор потока

    auto start_time = chrono::high_resolution_clock::now();

    cout << "Process ID: " << pid << endl;
    cout << "Thread ID:  " << tid << endl;
    cout << "Starting loop..." << endl << endl;

    for (int i = 1; i <= 1'000'000; ++i) {

        if (i % 1000 == 0) {
            // Определяем текущий CPU, на котором выполняется поток
            int cpu = sched_getcpu();

            // Уровень любезности (nice)
            int nice_val = getpriority(PRIO_PROCESS, 0);

            cout << "Iteration: " << i
                << " | PID: " << pid
                << " | TID: " << tid
                << " | nice: " << nice_val
                << " | CPU: " << cpu
                << endl;

            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    cout << endl << "Execution time: " << elapsed.count() << " seconds" << endl;
    cout << "Done." << endl;

    return 0;
}

