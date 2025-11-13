#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <cstdlib>
#include <sys/syscall.h>   
#include <sys/types.h>

void* Lab_04x(void* arg) {
    int N = *reinterpret_cast<int*>(arg);

    const char* username_env = getenv("USER");
    if (!username_env) {
        std::cerr << "Error: could not get username from environment variable USER." << std::endl;
        return nullptr;
    }

    std::string username(username_env);
    size_t name_len = username.length();
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);    

    for (int i = 0; i < N; ++i) {
        char letter = username[i % name_len];

        printf("PID: %d - TID: %d - Iteration: %d - Letter: %c\n",
            pid, tid, i + 1, letter);

        std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 100));
    }

    std::cout << "Thread (TID: " << tid << ") finished " << N << " iterations." << std::endl;
    return nullptr;
}


int main() {
    pthread_t thread1, thread2;
    int iter1 = 50;
    int iter2 = 125;

    if (pthread_create(&thread1, nullptr, Lab_04x, &iter1) != 0) {
        std::cerr << "Error: unable to create thread 1" << std::endl;
        return 1;
    }

    if (pthread_create(&thread2, nullptr, Lab_04x, &iter2) != 0) {
        std::cerr << "Error: unable to create thread 2" << std::endl;
        return 1;
    }

    int mainIter = 100;
    Lab_04x(&mainIter);

    if (pthread_join(thread1, nullptr) != 0) {
        std::cerr << "Error: unable to join thread 1" << std::endl;
    }

    if (pthread_join(thread2, nullptr) != 0) {
        std::cerr << "Error: unable to join thread 2" << std::endl;
    }

    std::cout << "Main thread finished after all child threads completed." << std::endl;
    return 0;
}

