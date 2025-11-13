#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

int main()
{
    setlocale(LC_ALL, "ru");

    DWORD currentPID = GetCurrentProcessId();

    std::cout << "PID текущего процесса: " << currentPID << std::endl;
    std::cout << "Список потоков этого процесса:\n\n";

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Ошибка при создании снимка. Код: " << GetLastError() << std::endl;
        return 1;
    }

    THREADENTRY32 te;
    te.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(hSnapshot, &te))
    {
        do
        {
            if (te.th32OwnerProcessID == currentPID)
            {
                std::cout << "-- Thread ID: " << te.th32ThreadID << std::endl;
            }
        } while (Thread32Next(hSnapshot, &te));
    }
    else
    {
        std::cerr << "Ошибка при получении информации о потоках. Код: " << GetLastError() << std::endl;
    }

    CloseHandle(hSnapshot);
    return 0;
}
