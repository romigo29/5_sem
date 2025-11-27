// Lab_05b_Linux.cpp
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <string>
#include <vector>
#include <cerrno>
#include <cstring>
#include <cstdlib>

using namespace std;

// Функция для установки affinity по маске (P1)
bool set_affinity(pid_t pid, unsigned long long mask_value) {
    cpu_set_t set;
    CPU_ZERO(&set);

    // Устанавливаем процессоры, на которых разрешено выполнение
    for (int i = 0; i < 64; ++i) {
        if (mask_value & (1ULL << i))
            CPU_SET(i, &set);
    }

    if (sched_setaffinity(pid, sizeof(set), &set) == -1) {
        cerr << "Failed to set affinity for PID " << pid << ": " << strerror(errno) << endl;
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./Lab_05b_Linux <P1> <P2> <P3>" << endl;
        cerr << "Example: ./Lab_05b_Linux 0xFF 0 0" << endl;
        return 1;
    }

    unsigned long long P1 = strtoull(argv[1], nullptr, 0); // маска процессоров
    int P2 = atoi(argv[2]);  // nice первого
    int P3 = atoi(argv[3]);  // nice второго

    cout << "Processor affinity mask: " << argv[1] << endl;
    cout << "Child 1 nice level: " << P2 << endl;
    cout << "Child 2 nice level: " << P3 << endl;

    string childProg = "./Lab_05x"; // дочерняя программа должна быть собрана

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Первый дочерний процесс
        if (!set_affinity(getpid(), P1))
            exit(1);

        if (setpriority(PRIO_PROCESS, 0, P2) == -1)
            cerr << "Failed to set nice for child 1: " << strerror(errno) << endl;

        execl(childProg.c_str(), childProg.c_str(), (char*)nullptr);
        cerr << "Failed to exec child 1: " << strerror(errno) << endl;
        exit(1);
    }
    else if (pid1 < 0) {
        cerr << "Failed to fork child 1" << endl;
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Второй дочерний процесс
        if (!set_affinity(getpid(), P1))
            exit(1);

        if (setpriority(PRIO_PROCESS, 0, P3) == -1)
            cerr << "Failed to set nice for child 2: " << strerror(errno) << endl;

        execl(childProg.c_str(), childProg.c_str(), (char*)nullptr); //+ утилита xterm
        cerr << "Failed to exec child 2: " << strerror(errno) << endl;
        exit(1);
    }
    else if (pid2 < 0) {
        cerr << "Failed to fork child 2" << endl;
        return 1;
    }

    cout << "Child processes started (PIDs: " << pid1 << ", " << pid2 << ")" << endl;

    // Ожидаем завершения обоих
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    cout << "Both child processes finished." << endl;
    return 0;
}
