#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: Lab_05b P1 P2 P3" << endl;
        return 1;
    }

    DWORD_PTR P1 = stoull(argv[1], nullptr, 0);
    DWORD P2 = stoul(argv[2], nullptr, 0);
    DWORD P3 = stoul(argv[3], nullptr, 0);


    cout << "Processor affinity mask: " << P1 << endl;
    cout << "Child 1 priority class: " << P2 << endl;
    cout << "Child 2 priority class: " << P3 << endl;

    LPCWSTR childExe = L"Lab_05x.exe";

    STARTUPINFO si1 = { sizeof(si1) };
    PROCESS_INFORMATION pi1;
    if (!CreateProcess(childExe, nullptr, nullptr, nullptr, FALSE,
        CREATE_NEW_CONSOLE, nullptr, nullptr, &si1, &pi1)) {
        cerr << "Failed to start child 1" << endl;
        return 1;
    }

    STARTUPINFO si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2;
    if (!CreateProcess(childExe, nullptr, nullptr, nullptr, FALSE,
        CREATE_NEW_CONSOLE, nullptr, nullptr, &si2, &pi2)) {
        cerr << "Failed to start child 2" << endl;
        TerminateProcess(pi1.hProcess, 1);
        return 1;
    }

    SetProcessAffinityMask(pi1.hProcess, P1);
    SetProcessAffinityMask(pi2.hProcess, P1);

    SetPriorityClass(pi1.hProcess, P2);
    SetPriorityClass(pi2.hProcess, P3);

    cout << "Child processes started." << endl;

    WaitForSingleObject(pi1.hProcess, INFINITE);
    WaitForSingleObject(pi2.hProcess, INFINITE);

    cout << "Both child processes finished." << endl;

    CloseHandle(pi1.hProcess);
    CloseHandle(pi1.hThread);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

    return 0;
}
