#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

bool isPrime(unsigned long long n) {
    if (n < 2) return false;
    for (unsigned long long i = 2; i <= static_cast<unsigned long long>(std::sqrt(n)); ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: Lab_07x <duration_seconds>" << std::endl;
        return 1;
    }

    int durationSec = std::atoi(argv[1]);
    if (durationSec <= 0) durationSec = 10;

    unsigned long long counter = 0;
    unsigned long long num = 2;

    timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start);

    while (true) {
        if (isPrime(num)) {
            ++counter;
        }
        ++num;

        clock_gettime(CLOCK_MONOTONIC, &current);
        double elapsed =
            current.tv_sec - start.tv_sec +
            (current.tv_nsec - start.tv_nsec) / 1e9;

        if (elapsed >= durationSec)
            break;
    }

    clock_gettime(CLOCK_MONOTONIC, &current);
    double elapsed =
        current.tv_sec - start.tv_sec +
        (current.tv_nsec - start.tv_nsec) / 1e9;

    std::cout << "Total iterations: " << counter << std::endl;
    std::cout << "Actual runtime: " << elapsed << " seconds" << std::endl;

    std::cout << "\nPress Enter to exit..." << std::endl;
    std::cin.get();   // ожидание нажатия клавиши

    return 0;
}
