#include <Windows.h>
#include <iostream>

int main() {
    STARTUPINFO si1 = { sizeof(si1) };
    PROCESS_INFORMATION pi1;

    STARTUPINFO si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2;

    wchar_t cmd1[] = L"Lab_07x.exe 60";  // первый процесс 1 минута
    wchar_t cmd2[] = L"Lab_07x.exe 120"; // второй процесс 2 минуты


    // Запуск первого дочернего процесса (1 минута)
    if (!CreateProcessW(NULL, cmd1, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si1, &pi1)) {
        std::cerr << "Failed to start first child process!" << std::endl;
        return 1;
    }

    // Запуск второго дочернего процесса (2 минуты)
    if (!CreateProcessW(NULL, cmd2, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si2, &pi2)) {
        std::cerr << "Failed to start second child process!" << std::endl;
        // Завершаем первый процесс
        TerminateProcess(pi1.hProcess, 1);
        CloseHandle(pi1.hProcess);
        CloseHandle(pi1.hThread);
        return 1;
    }

    // Ограничение времени работы дочерних процессов
    // 1 минута = 60 000 мс, 2 минуты = 120 000 мс
    WaitForSingleObject(pi1.hProcess, 60000); // ждем до 1 минуты
  

    WaitForSingleObject(pi2.hProcess, 120000); // ждем до 2 минут


    // Ждем, чтобы процессы корректно завершились
    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    // Закрываем дескрипторы
    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    std::cout << "Both child processes finished." << std::endl;
    system("pause");
    return 0;
}
