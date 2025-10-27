#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>    

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

int main(int argc, char* argv[])
{
    int nIterations = 0;

    if (argc > 1) {
        nIterations = std::atoi(argv[1]);
    }
    else {
        const char* envVal = std::getenv("ITER_NUM");
        if (envVal) nIterations = std::atoi(envVal);
    }

    if (nIterations <= 0) {
        std::cerr << "Error: number of iterations is not specified.\n";
#ifdef _WIN32
        ExitProcess(1);
#else
        std::exit(1);
#endif
    }

    std::cout << "Number of iterations: " << nIterations << std::endl;

#ifdef _WIN32
    DWORD pid = GetCurrentProcessId();
#else
    pid_t pid = getpid();
#endif

    for (int i = 0; i < nIterations; ++i) {
        std::cout << "Iteration " << (i + 1)
            << " - PID: " << pid << std::endl;

#ifdef _WIN32
        Sleep(500);  
#else
        usleep(500000);  
#endif
    }

    return 0;
}
