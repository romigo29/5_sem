#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <cstdlib>

int main() {
    setenv("LANG", "C", 1); // English error messages

    pid_t pid1 = fork();
    if (pid1 == 0) {
        execl("./Lab-03x", "Lab-03x", "5", NULL);
        perror("Error execl first process");
        return 1;
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        setenv("ITER_NUM", "10", 1);
        execl("./Lab-03x", "Lab-03x", NULL);
        perror("Error execl second process");
        return 1;
    }

    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);

    std::cout << "Both child processes finished.\n";
    return 0;
}
