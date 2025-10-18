#include "../OS11_HTAPI/HT.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <Windows.h>

using namespace std;

bool isStartAlive(DWORD startPID)
{
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, startPID);
    if (!hProc) return false;

    DWORD exitCode;
    bool alive = GetExitCodeProcess(hProc, &exitCode) && exitCode == STILL_ACTIVE;
    CloseHandle(hProc);
    return alive;
}

bool tryConnectToStart(DWORD& startPID, HTHANDLE*& h, const char* filename)
{
    HANDLE hMutex = OpenMutexA(SYNCHRONIZE, FALSE, "HTAPI_MUTEX");
    if (!hMutex)
        return false;

    HANDLE hMap = OpenFileMappingA(FILE_MAP_READ, FALSE, "HTAPI_PID_MAP");
    if (!hMap) {
        CloseHandle(hMutex);
        return false;
    }

    DWORD* pPid = (DWORD*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, sizeof(DWORD));
    if (!pPid) {
        CloseHandle(hMap);
        CloseHandle(hMutex);
        return false;
    }

    startPID = *pPid;
    UnmapViewOfFile(pPid);
    CloseHandle(hMap);

    if (!isStartAlive(startPID)) {
        CloseHandle(hMutex);
        return false;
    }

    h = Open(filename);
    if (!h) {
        CloseHandle(hMutex);
        return false;
    }

    CloseHandle(hMutex);
    return true;
}

int main(int argc, char** argv)
{
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const char* filename = argv[1];
    HTHANDLE* h = nullptr;
    DWORD startPID = 0;

    srand((unsigned)time(NULL));

    while (true)
    {
        if (!h || !isStartAlive(startPID)) {
            if (h) {
                Close(h);
                h = nullptr;
                cout << "[WAIT] OS11_START был закрыт. Ожидание повторного запуска...\n";
            }

            while (!tryConnectToStart(startPID, h, filename)) {
                this_thread::sleep_for(chrono::seconds(1));
            }

            cout << "[INFO] Подключились к OS11_START PID=" << startPID << "\n";
        }

        int key = rand() % 50;
        int value = 0;
        Element e(&key, sizeof(key), &value, sizeof(value));
        if (!Insert(h, &e)) {
            cerr << "[INSERT FAIL] key=" << key << " value=" << value << ": " << HT_GetLastError(h) << endl;
        }
        else {
            cout << "[INSERT] key=" << key << " value=" << value << endl;
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    if (h)
        Close(h);

    return 0;
}
