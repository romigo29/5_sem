#include <windows.h>
#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>

CRITICAL_SECTION g_cs;
std::wstring g_letters;        

std::wstring GetComputerNameFromEnv()
{
    wchar_t buffer[256];
    DWORD size = GetEnvironmentVariableW(L"USERNAME", buffer, 256);

    if (size > 0 && size < 256)
        return std::wstring(buffer);
    else
        return L"UNKNOWN_PC";
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    std::wstring threadName = *(std::wstring*)lpParam;
    delete (std::wstring*)lpParam;

    for (int i = 1; i <= 90; ++i)
    {
        if (i == 30) EnterCriticalSection(&g_cs);

        wchar_t ch = g_letters[(i - 1) % g_letters.size()];
        std::wcout << threadName << L" – " << i << L" – " << ch << L'\n';

        if (i == 60) LeaveCriticalSection(&g_cs);

        Sleep(100);
    }

    return 0;
}

int wmain()
{
    _setmode(_fileno(stdout), _O_U16TEXT);

    InitializeCriticalSection(&g_cs);

    std::wstring compName = GetComputerNameFromEnv();

    g_letters = compName;

    HANDLE hThreadA = CreateThread(
        nullptr, 0, ThreadProc, new std::wstring(L"A"), 0, nullptr);

    HANDLE hThreadB = CreateThread(
        nullptr, 0, ThreadProc, new std::wstring(L"B"), 0, nullptr);

    for (int i = 1; i <= 90; ++i)
    {
        if (i == 30) EnterCriticalSection(&g_cs);

        wchar_t ch = g_letters[(i - 1) % g_letters.size()];
        std::wcout << L"main – " << i << L" – " << ch << L'\n';

        if (i == 60) LeaveCriticalSection(&g_cs);

        Sleep(100);
    }

    HANDLE threads[2] = { hThreadA, hThreadB };
    WaitForMultipleObjects(2, threads, TRUE, INFINITE);

    CloseHandle(hThreadA);
    CloseHandle(hThreadB);

    DeleteCriticalSection(&g_cs);
    return 0;
}
