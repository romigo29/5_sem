#include <windows.h>
#include <iostream>
#include <bitset>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

int main() {
    DWORD processId = GetCurrentProcessId();
    HANDLE hProcess = GetCurrentProcess();
    DWORD_PTR processAffinityMask = 0, systemAffinityMask = 0;

    if (!GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask)) {
        cerr << "Ошибка получения маски родственности" << endl;
        return 1;
    }

    clock_t startTime = clock();

    for (int i = 1; i <= 1000000; i++) {
        if (i % 1000 == 0) {
            DWORD threadId = GetCurrentThreadId();
            int threadPriority = GetThreadPriority(GetCurrentThread());
            DWORD processPriorityClass = GetPriorityClass(hProcess);
            DWORD currentProcessor = GetCurrentProcessorNumber();

            cout << "Iteration: " << i << endl;
            cout << "Process ID: " << processId << endl;
            cout << "Thread ID: " << threadId << endl;
            cout << "Process priority class: " << processPriorityClass << endl;
            cout << "Thread priority: " << threadPriority << endl;
            cout << "Current processor: " << currentProcessor << endl;
            cout << "-----------------------------" << endl;

            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    clock_t endTime = clock();
    double elapsed = double(endTime - startTime) / CLOCKS_PER_SEC;
    cout << "Elapsed time: " << elapsed << " seconds" << endl;

    system("pause");

    return 0;
}
