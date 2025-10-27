#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

int main() {

    setlocale(LC_ALL, "ru");
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Не удалось создать снимок процессов.\n";
        return 1;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnapshot, &pe32)) {
        do {
            std::wcout << "Имя: " << pe32.szExeFile
                << " | PID: " << pe32.th32ProcessID
                << " | Parent PID: " << pe32.th32ParentProcessID
                << "\n";
        } while (Process32Next(hSnapshot, &pe32));
    }
    else {
        std::cout << "Не удалось получить первый процесс.\n";
    }

    CloseHandle(hSnapshot);
    return 0;
}
