#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

wstring username;      
int usernameIndex = 0;

wchar_t NextUserChar() {
    wchar_t ch = username[usernameIndex];
    usernameIndex = (usernameIndex + 1) % username.size();   
    return ch;
}

wstring GetUsername() {
    wchar_t buffer[256];
    DWORD size = GetEnvironmentVariableW(L"USERNAME", buffer, 256);
    if (size == 0) return L"USER";
    return buffer;
}

void Work(const wstring& name, HANDLE hEvent, bool isParent) {
    if (!isParent) {
        WaitForSingleObject(hEvent, INFINITE);
    }

    for (int i = 1; i <= 90; i++) {
        wcout << name << L": iter " << i << L" (" << NextUserChar() << L")\n";
        Sleep(300);
    }
}

int wmain(int argc, wchar_t* argv[]) {
    username = GetUsername();
    if (username.empty()) username = L"USER";

    if (argc == 2 && (wcscmp(argv[1], L"A") == 0 || wcscmp(argv[1], L"B") == 0)) {
        HANDLE hEvent = OpenEventW(SYNCHRONIZE, FALSE, L"StartEvent");
        Work(argv[1], hEvent, false);
        return 0;
    }

    HANDLE hEvent = CreateEventW(NULL, TRUE, FALSE, L"StartEvent");

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    PROCESS_INFORMATION piA{}, piB{};

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    std::wstring cmdA = std::wstring(L"\"") + exePath + L"\" A";
    std::wstring cmdB = std::wstring(L"\"") + exePath + L"\" B";

    CreateProcessW(NULL, &cmdA[0], NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piA);
    CreateProcessW(NULL, &cmdB[0], NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &piB);

    for (int i = 1; i <= 15; i++) {
        wcout << L"maiN: iter " << i << L" (" << NextUserChar() << L")\n";
        Sleep(300);
    }

    SetEvent(hEvent);

    for (int i = 16; i <= 90; i++) {
        wcout << L"maiN: iter " << i << L" (" << NextUserChar() << L")\n";
        Sleep(300);
    }

    WaitForSingleObject(piA.hProcess, INFINITE);
    WaitForSingleObject(piB.hProcess, INFINITE);

    CloseHandle(piA.hProcess);
    CloseHandle(piA.hThread);
    CloseHandle(piB.hProcess);
    CloseHandle(piB.hThread);
    CloseHandle(hEvent);

    return 0;
}
