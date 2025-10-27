#include <windows.h>
#include <iostream>

int main() {

    setlocale(LC_ALL, "ru");

    STARTUPINFO si[3];
    PROCESS_INFORMATION pi[3];
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));

    for (int i = 0; i < 3; ++i) {
        si[i].cb = sizeof(si[i]);
    }

    LPCSTR appName = "Lab-03x.exe";

    SetEnvironmentVariableA("ITER_NUM", "40");

    if (!CreateProcessA(
        "Lab-03x.exe",
        NULL,
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si[0], &pi[0]))
    {
        std::cout << "Ошибка создания первого процесса";
        pi[0].hProcess = NULL;
    }

    if (!CreateProcessA(
        NULL,
        (LPSTR)"Lab-03x.exe 50",
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL,
        &si[1], &pi[1]))
    {
        std::cout << "Ошибка создания второго процесса";
        pi[1].hProcess = NULL;
    }

    char envBlock[] = "ITER_NUM=30\0";     
    LPVOID lpEnvironment = envBlock;

    if (!CreateProcessA(
        appName,
        NULL,
        NULL, NULL, FALSE, CREATE_NEW_CONSOLE, lpEnvironment, NULL,
        &si[2], &pi[2]))
    {
        std::cout << "Ошибка создания третьего процесса";
        pi[2].hProcess = NULL;
    }

    HANDLE handles[3];
    int count = 0;
    for (int i = 0; i < 3; ++i) {
        if (pi[i].hProcess) {
            handles[count++] = pi[i].hProcess;
        }
    }

    if (count > 0) {
        WaitForMultipleObjects(count, handles, TRUE, INFINITE);
    }

    for (int i = 0; i < 3; ++i) {
        if (pi[i].hProcess) CloseHandle(pi[i].hProcess);
        if (pi[i].hThread) CloseHandle(pi[i].hThread);
    }

    std::cout << "Все дочерние процессы завершены.\n";
    return 0;
}
