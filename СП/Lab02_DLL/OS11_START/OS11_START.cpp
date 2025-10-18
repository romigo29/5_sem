#include "../OS11_HTAPI/HT.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <conio.h>
#include <Windows.h>

using namespace std;

int main(int argc, char** argv)
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    if (argc < 2) {
        cout << "Usage: OS11_START <filename>\n";
        return 1;
    }

    const char* filename = argv[1];
    HTHANDLE* h = Open(filename);
    if (!h) {
        cerr << "Open failed: " << HT_GetLastError(nullptr) << endl;
        return 1;
    }

    HANDLE hMutex = CreateMutexA(NULL, FALSE, "HTAPI_MUTEX");
    if (!hMutex) {
        cerr << "Failed to create mutex: " << GetLastError() << endl;
        Close(h);
        return 1;
    }

    HANDLE hMap = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(DWORD), "HTAPI_PID_MAP");
    if (!hMap) {
        cerr << "Failed to create file mapping: " << GetLastError() << endl;
        CloseHandle(hMutex);
        Close(h);
        return 1;
    }

    DWORD* pPid = (DWORD*)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, sizeof(DWORD));
    if (pPid) {
        *pPid = GetCurrentProcessId();
        UnmapViewOfFile(pPid);
    }

    Snap(h);

    cout << "HT-Storage Start filename=" << filename
        << ", snapshotinterval=" << h->SecSnapshotInterval
        << ", capacity=" << h->Capacity
        << ", maxkeylength=" << h->MaxKeyLength
        << ", maxdatalength=" << h->MaxPayloadLength << endl;

    atomic<bool> running(true);
    condition_variable cv;
    mutex cv_m;

    thread snapshotThread([&]() {
        unique_lock<mutex> lock(cv_m);
        while (running.load()) {
            if (cv.wait_for(lock, chrono::seconds(h->SecSnapshotInterval),
                [&]() { return !running.load(); })) {
                break;
            }

            if (!Snap(h)) {
                cerr << "Snap failed: " << HT_GetLastError(h) << endl;
            }
        }
        });

    cout << "Press any key to stop..." << endl;

    while (running.load()) {
        if (_kbhit()) {
            _getch();
            running.store(false);
            cv.notify_all();
            break;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    if (snapshotThread.joinable())
        snapshotThread.join();

    Snap(h);
    Close(h);

    CloseHandle(hMap);
    CloseHandle(hMutex);

    cout << "OS11_START stopped." << endl;
    return 0;
}
