#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <locale.h>
#include <thread>
#include <chrono>

void* Lab_04x(void* arg)
{
    int N = *reinterpret_cast<int*>(arg);

    const char* username_env = getenv("USER");
    if (!username_env) {
        std::cerr << "Error: could not get username from environment variable USER." << std::endl;
        return nullptr;
    }

    std::string username(username_env);
    size_t name_len = username.length();
    pid_t pid = getpid();
    pthread_t tid = pthread_self();

    for (int i = 0; i < N; ++i)
    {
        char letter = username[i % name_len];

        printf("PID: %d - TID: %lu - Iteration: %d - Letter: %c\n",
            pid, tid, i + 1, letter);

        std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 100));
    }

    return nullptr;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <number_of_iterations>\n";
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        std::cerr << "Error: number of iterations must be a positive integer." << std::endl;
        return 1;
    }

    pthread_t thread;
    if (pthread_create(&thread, nullptr, Lab_04x, &N) != 0) {
        std::cerr << "Error: failed to create thread." << std::endl;
        return 1;
    }

    pthread_join(thread, nullptr);
    std::cout << "Thread finished successfully." << std::endl;

    return 0;
}

