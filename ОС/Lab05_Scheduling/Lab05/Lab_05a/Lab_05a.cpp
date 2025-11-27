#include <windows.h>
#include <iostream>
#include <bitset>
using namespace std;

int main() {

    setlocale(LC_ALL, "ru");

    DWORD processId = GetCurrentProcessId();
    DWORD threadId = GetCurrentThreadId();

    HANDLE hProcess = GetCurrentProcess();

    DWORD processPriorityClass = GetPriorityClass(hProcess);
    int threadPriority = GetThreadPriority(GetCurrentThread());

    DWORD_PTR processAffinityMask = 0, systemAffinityMask = 0;
    if (!GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask)) {
        cerr << "Ошибка получения маски родственности" << endl;
        return 1;
    }

    int availableProcessors = 0;
    for (DWORD_PTR mask = processAffinityMask; mask; mask >>= 1) {
        if (mask & 1) availableProcessors++;
    }

    DWORD currentProcessor = GetCurrentProcessorNumber();

    cout << "Process ID: " << processId << endl;
    cout << "Thread ID: " << threadId << endl;

    cout << "Process priority class: " << processPriorityClass << endl;
    cout << "Thread priority: " << threadPriority << endl;

    cout << "Process affinity mask: " << bitset<32>(processAffinityMask) << endl;
    cout << "System affinity mask: " << bitset<32>(systemAffinityMask) << endl;

    cout << "Available processors: " << availableProcessors << endl;
    cout << "Current processor: " << currentProcessor << endl;

    return 0;
}
