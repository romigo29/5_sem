#include <windows.h>
#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>

const wchar_t* MUTEX_NAME = L"Lab06b_Mutex_Name";      
std::wstring g_letters;      

std::wstring GetComputerNameFromEnv()
{
    wchar_t buffer[256];
    DWORD size = GetEnvironmentVariableW(L"USERNAME", buffer, 256);
    if (size > 0 && size < 256) return std::wstring(buffer);
    return std::wstring(L"UNKNOWNPC");
}

void RunMainLoop(const std::wstring& threadName)
{
    HANDLE hMutex = CreateMutexW(nullptr, FALSE, MUTEX_NAME);
    if (!hMutex)
    {
        std::wcerr << threadName << L" Ц ќшибка CreateMutex: " << GetLastError() << L'\n';
        return;
    }

    for (int i = 1; i <= 90; ++i)
    {
        if (i == 30)
        {
            DWORD wait = WaitForSingleObject(hMutex, INFINITE);
            if (wait != WAIT_OBJECT_0)
            {
                std::wcerr << threadName << L" Ц WaitForSingleObject failed: " << GetLastError() << L'\n';
                break;
            }
            std::wcout << threadName << L" Ц entering protected region (30)\n";
        }

        wchar_t ch = g_letters[(i - 1) % g_letters.size()];
        std::wcout << threadName << L" Ц " << i << L" Ц " << ch << L'\n';

        if (i == 60)
        {
            if (!ReleaseMutex(hMutex))
            {
                std::wcerr << threadName << L" Ц ReleaseMutex failed: " << GetLastError() << L'\n';
            }
            else
            {
                std::wcout << threadName << L" Ц left protected region (60)\n";
            }
        }

        Sleep(100);
    }

    CloseHandle(hMutex);
}

int wmain(int argc, wchar_t* argv[])
{

    g_letters = GetComputerNameFromEnv();
    if (g_letters.empty()) g_letters = L"PC";

    if (argc >= 2)
    {
        std::wstring arg = argv[1];
        if (arg == L"A" || arg == L"B")
        {
            std::wstring procName = (arg == L"A") ? L"A" : L"B";
            RunMainLoop(procName);
            return 0;
        }
    }

    wchar_t modulePath[MAX_PATH];
    GetModuleFileNameW(nullptr, modulePath, MAX_PATH);

    std::wstring cmdA = std::wstring(L"\"") + modulePath + L"\" A";
    std::wstring cmdB = std::wstring(L"\"") + modulePath + L"\" B";

    STARTUPINFOW siA = { sizeof(siA) };
    PROCESS_INFORMATION piA = { 0 };
    STARTUPINFOW siB = { sizeof(siB) };
    PROCESS_INFORMATION piB = { 0 };

    BOOL okA = CreateProcessW(
        nullptr,
        &cmdA[0],
        nullptr, nullptr,
        FALSE,
        CREATE_NEW_CONSOLE,
        nullptr,
        nullptr,
        &siA,
        &piA);

    if (!okA)
    {
        std::wcerr << L"Ќе удалось создать процесс A. ќшибка: " << GetLastError() << L'\n';
    }

    BOOL okB = CreateProcessW(
        nullptr,
        &cmdB[0],
        nullptr, nullptr,
        FALSE,
        CREATE_NEW_CONSOLE,
        nullptr,
        nullptr,
        &siB,
        &piB);

    if (!okB)
    {
        std::wcerr << L"Ќе удалось создать процесс B. ќшибка: " << GetLastError() << L'\n';
    }

    RunMainLoop(L"main");

    HANDLE waitHandles[2];
    int count = 0;
    if (okA) { waitHandles[count++] = piA.hProcess; }
    if (okB) { waitHandles[count++] = piB.hProcess; }

    if (count > 0)
    {
        WaitForMultipleObjects(count, waitHandles, TRUE, INFINITE);
    }

    if (okA)
    {
        CloseHandle(piA.hProcess);
        CloseHandle(piA.hThread);
    }
    if (okB)
    {
        CloseHandle(piB.hProcess);
        CloseHandle(piB.hThread);
    }

    return 0;
}
