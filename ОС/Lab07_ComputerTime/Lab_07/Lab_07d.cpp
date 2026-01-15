#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>

int main() {
    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return 1;
    }

    if (pid1 == 0) {
        execlp(
            "xterm",
            "xterm",
            "-hold",
            "-e",
            "./Lab_07x",
            "60",
            nullptr
        );
        perror("execlp");
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return 1;
    }

    if (pid2 == 0) {
        execlp(
            "xterm",
            "xterm",
            "-hold",
            "-e",
            "./Lab_07x",
            "120",
            nullptr
        );
        perror("execlp");
        return 1;
    }

    int status;
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);

    std::cout << "Both child processes finished." << std::endl;
    return 0;
}
