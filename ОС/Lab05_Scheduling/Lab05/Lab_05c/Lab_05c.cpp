#include <windows.h>
#include <iostream>
#include <string>
#include <ctime>

using namespace std;

DWORD WINAPI ThreadFunc(LPVOID param) {
    int id = *reinterpret_cast<int*>(param);

    clock_t start = clock();

    for (int i = 1; i <= 1000000; i++) {
        if (i % 1000 == 0) {
            Sleep(200);
            cout << "Thread " << id
                << " iteration: " << i
                << " | Process ID: " << GetCurrentProcessId()
                << " | Thread ID: " << GetCurrentThreadId()
                << " | CPU: " << GetCurrentProcessorNumber()
                << endl;
        }
    }

    double time = double(clock() - start) / CLOCKS_PER_SEC;
    cout << "Thread " << id << " finished in " << time << " sec" << endl;

    system("pause");
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        cerr << "Usage: Lab_05c P1 P2 P3 P4" << endl;
        return 1;
    }

    DWORD_PTR P1 = stoull(argv[1], nullptr, 0);
    DWORD P2 = stoul(argv[2], nullptr, 0);
    int P3 = stoi(argv[3]);
    int P4 = stoi(argv[4]);

    cout << "Processor affinity mask: " << P1 << endl;
    cout << "Process priority class: " << P2 << endl;
    cout << "Thread 1 priority: " << P3 << endl;
    cout << "Thread 2 priority: " << P4 << endl;

    SetProcessAffinityMask(GetCurrentProcess(), P1);
    SetPriorityClass(GetCurrentProcess(), P2);

    int id1 = 1, id2 = 2;
    HANDLE hThread1 = CreateThread(nullptr, 0, ThreadFunc, &id1, 0, nullptr);
    HANDLE hThread2 = CreateThread(nullptr, 0, ThreadFunc, &id2, 0, nullptr);

    SetThreadPriority(hThread1, P3);
    SetThreadPriority(hThread2, P4);

    WaitForSingleObject(hThread1, INFINITE);
    WaitForSingleObject(hThread2, INFINITE);

    CloseHandle(hThread1);
    CloseHandle(hThread2);

    cout << "Both threads finished." << endl;
    return 0;
}
