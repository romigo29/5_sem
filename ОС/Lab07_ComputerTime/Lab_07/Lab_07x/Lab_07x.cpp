#include <Windows.h>
#include <iostream>
#include <cstdlib>

// Функция проверки простого числа
bool isPrime(unsigned long long n) {
    if (n < 2) return false;
    for (unsigned long long i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int wmain(int argc, wchar_t* argv[]) {
    if (argc < 2) {
        std::wcerr << L"Usage: Lab-07x <duration_seconds>" << std::endl;
        return 1;
    }

    int durationSec = _wtoi(argv[1]);
    if (durationSec <= 0) durationSec = 10;

    unsigned long long counter = 0;
    unsigned long long num = 2;

    ULONGLONG startTime = GetTickCount64();

    while (true) {
        if (isPrime(num)) {
            ++counter;
            std::wcout << counter << L": " << num << std::endl;
        }
        ++num;

        ULONGLONG elapsed = (GetTickCount64() - startTime) / 1000;
        if (elapsed >= (ULONGLONG)durationSec)
            break;
    }

    ULONGLONG endTime = GetTickCount64();

    std::wcout << L"Total primes found: " << counter << std::endl;
    std::wcout << L"Actual runtime: "
        << (endTime - startTime) / 1000.0
        << L" seconds" << std::endl;

    std::wcout << L"\nPress Enter to close this console..." << std::endl;
    std::wcin.get();   // ожидание нажатия клавиши

    return 0;
}
