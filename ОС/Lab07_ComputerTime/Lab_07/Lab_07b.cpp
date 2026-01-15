#include <iostream>
#include <ctime>

int main() {
    unsigned long long counter = 0;

    struct timespec start, current;
    clock_gettime(CLOCK_MONOTONIC, &start); // фиксируем старт

    bool reported5 = false;
    bool reported10 = false;

    while (true) {
        counter++;

        clock_gettime(CLOCK_MONOTONIC, &current);
        double elapsed = current.tv_sec - start.tv_sec + (current.tv_nsec - start.tv_nsec) / 1e9;

        if (!reported5 && elapsed >= 5.0) {
            std::cout << "Iterations after 5 seconds: " << counter << std::endl;
            reported5 = true;
        }
        if (!reported10 && elapsed >= 10.0) {
            std::cout << "Iterations after 10 seconds: " << counter << std::endl;
            reported10 = true;
        }
        if (elapsed >= 15.0) {
            std::cout << "Final iterations after 15 seconds: " << counter << std::endl;
            std::cout << "Actual runtime: " << elapsed << " seconds" << std::endl;
            break;
        }
    }

    return 0;
}
