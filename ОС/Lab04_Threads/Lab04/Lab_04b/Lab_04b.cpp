#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

DWORD WINAPI Lab_04x(LPVOID lpParam)
{
    int N = *reinterpret_cast<int*>(lpParam);

    char usernameBuffer[256];
    DWORD len = GetEnvironmentVariableA("USERNAME", usernameBuffer, sizeof(usernameBuffer));

    if (len == 0 || len > sizeof(usernameBuffer)) {
        fprintf(stderr, "Ошибка: невозможно получить имя пользователя из переменной окружения USERNAME.\n");
        return 1;
    }

    std::string username(usernameBuffer);
    const size_t name_len = username.length();

    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    for (int i = 0; i < N; ++i)
    {
        char letter = username[i % name_len];
        printf("PID: %lu – TID: %lu – Итерация: %d – Буква: %c\n",
            pid, tid, i + 1, letter);
        std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 100));
    }

    return 0;
}

int main()
{
    setlocale(LC_ALL, "ru");

    int N_thread1 = 50;
    int N_thread2 = 125;
    int N_main = 100;

    DWORD threadId1, threadId2;
    HANDLE hThread1 = CreateThread(nullptr, 0, Lab_04x, &N_thread1, 0, &threadId1);
    HANDLE hThread2 = CreateThread(nullptr, 0, Lab_04x, &N_thread2, 0, &threadId2);

    if (!hThread1 || !hThread2) {
        std::cerr << "Ошибка при создании потоков. Код: " << GetLastError() << std::endl;
        if (hThread1) CloseHandle(hThread1);
        if (hThread2) CloseHandle(hThread2);
        return 1;
    }

    char usernameBuffer[256];
    DWORD len = GetEnvironmentVariableA("USERNAME", usernameBuffer, sizeof(usernameBuffer));
    std::string username = (len > 0) ? usernameBuffer : "User";
    size_t name_len = username.length();
    DWORD pid = GetCurrentProcessId();
    DWORD tid = GetCurrentThreadId();

    for (int i = 0; i < N_main; ++i)
    {
        char letter = username[i % name_len];
        printf("PID: %lu – MAIN – Итерация: %d – Буква: %c\n", pid, i + 1, letter);

        if (i + 1 == 20) {
            std::cout << "\n--- Приостановка первого потока ---\n";
            SuspendThread(hThread1);
        }
        if (i + 1 == 60) {
            std::cout << "\n--- Возобновление первого потока ---\n";
            ResumeThread(hThread1);
        }
        if (i + 1 == 40) {
            std::cout << "\n--- Приостановка второго потока ---\n";
            SuspendThread(hThread2);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 100));
    }

    std::cout << "\n==> Возобновление второго потока (после завершения цикла)\n";
    ResumeThread(hThread2);

    HANDLE threads[] = { hThread1, hThread2 };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    std::cout << "Все потоки завершили работу.\n";
    return 0;
}
