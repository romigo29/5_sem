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
        try {
            char letter = username[i % name_len];

            printf("PID: %lu – TID: %lu – Итерация: %d – Буква: %c\n",
                pid, tid, i + 1, letter);

            std::this_thread::sleep_for(std::chrono::milliseconds(300 + rand() % 100));
        }
        catch (const std::exception& ex) {
            fprintf(stderr, "Ошибка: %s\n", ex.what());
            return 1;
        }
    }

    return 0;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cout << "Использование: " << argv[0] << " <количество_итераций>\n";
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        std::cout << "Ошибка: количество итераций должно быть положительным числом.\n";
        return 1;
    }

    DWORD threadId;
    HANDLE hThread = CreateThread(
        nullptr,                
        0,                       
        Lab_04x,               
        &N,                   
        0,                     
        &threadId);            

    if (hThread == nullptr) {
        std::cerr << "Ошибка создания потока. Код: " << GetLastError() << std::endl;
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    std::cout << "Работа потока завершена успешно.\n";
    return 0;
}
