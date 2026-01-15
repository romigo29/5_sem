#include <windows.h>
#include <iostream>
#include <string>
#include <io.h>
#include <fcntl.h>

const wchar_t* SEM_NAME = L"Lab06c_BinarySemaphore";    

std::wstring g_letters;    

std::wstring GetComputerNameFromEnv()
{
    wchar_t buffer[256];
    DWORD size = GetEnvironmentVariableW(L"USERNAME", buffer, 256);
    if (size > 0 && size < 256) return std::wstring(buffer);
    return L"UNKNOWNPC";
}

void RunMainLoop(const std::wstring& processName)
{
    HANDLE hSem = CreateSemaphoreW(
        nullptr,   
        1,               
        1,              
        SEM_NAME   
    );

    if (!hSem)
    {
        std::wcerr << processName << L" – CreateSemaphore ERROR: " << GetLastError() << L'\n';
        return;
    }

    for (int i = 1; i <= 90; ++i)
    {
        if (i == 30)
        {
            DWORD wait = WaitForSingleObject(hSem, INFINITE);
            if (wait != WAIT_OBJECT_0)
            {
                std::wcerr << processName << L" – WaitForSingleObject ERROR: " << GetLastError() << L'\n';
                break;
            }
            std::wcout << processName << L" – ENTER critical region (30)\n";
        }

        wchar_t ch = g_letters[(i - 1) % g_letters.size()];
        std::wcout << processName << L" – " << i << L" – " << ch << L'\n';

        if (i == 60)
        {
            if (!ReleaseSemaphore(hSem, 1, nullptr))
            {
                std::wcerr << processName << L" – ReleaseSemaphore ERROR: " << GetLastError() << L'\n';
            }
            else
            {
                std::wcout << processName << L" – LEAVE critical region (60)\n";
            }
        }

        Sleep(100);
    }

    CloseHandle(hSem);
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
            RunMainLoop(arg);
            return 0;
        }
    }

    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    std::wstring cmdA = std::wstring(L"\"") + exePath + L"\" A";
    std::wstring cmdB = std::wstring(L"\"") + exePath + L"\" B";

    STARTUPINFOW siA{ sizeof(siA) }, siB{ sizeof(siB) };
    PROCESS_INFORMATION piA{}, piB{};

    BOOL okA = CreateProcessW(nullptr, &cmdA[0], nullptr, nullptr,
        FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &siA, &piA);

    BOOL okB = CreateProcessW(nullptr, &cmdB[0], nullptr, nullptr,
        FALSE, CREATE_NEW_CONSOLE, nullptr, nullptr, &siB, &piB);

    if (!okA) std::wcerr << L"Ошибка создания A: " << GetLastError() << L"\n";
    if (!okB) std::wcerr << L"Ошибка создания B: " << GetLastError() << L"\n";

    RunMainLoop(L"main");

    HANDLE arr[2];
    int count = 0;
    if (okA) arr[count++] = piA.hProcess;
    if (okB) arr[count++] = piB.hProcess;

    if (count > 0) WaitForMultipleObjects(count, arr, TRUE, INFINITE);

    if (okA)
    {
        CloseHandle(piA.hThread);
        CloseHandle(piA.hProcess);
    }
    if (okB)
    {
        CloseHandle(piB.hThread);
        CloseHandle(piB.hProcess);
    }

    return 0;
}
