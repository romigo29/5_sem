#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


using namespace std;

string g_letters;
sem_t* g_sem;

// Получение имени пользователя
string GetUserName() {
    const char* user = getenv("USER");
    return user ? string(user) : "USER";
}

// Основной цикл процесса
void RunLoop(const string& name) {
    for (int i = 1; i <= 90; ++i) {
        if (i == 30) {
            sem_wait(g_sem); // вход в критическую секцию
            cout << name << " – ENTER critical region (30)" << endl;
        }

        char ch = g_letters[(i - 1) % g_letters.size()];
        cout << name << " – " << i << " – " << ch << endl;

        if (i == 60) {
            sem_post(g_sem); // выход
            cout << name << " – LEAVE critical region (60)" << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main(int argc, char* argv[]) {
    g_letters = GetUserName();
    if (g_letters.empty()) g_letters = "USER";

    // Создаём именованный бинарный семафор
    g_sem = sem_open("/Lab_06bSem", O_CREAT, 0666, 1);
    if (g_sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    // Если аргумент A или B → дочерний процесс
    if (argc == 2) {
        string procName = argv[1];
        RunLoop(procName);
        sem_close(g_sem);
        return 0;
    }

    // Родительский процесс main
    pid_t pidA = fork();
    if (pidA == 0) {
        // Запуск дочернего процесса A в отдельном окне
        execlp("xterm", "xterm", "-e", "./Lab_06b", "A", nullptr);
        perror("execlp A failed");
        return 1;
    }

    pid_t pidB = fork();
    if (pidB == 0) {
        // Запуск дочернего процесса B в отдельном окне
        execlp("xterm", "xterm", "-e", "./Lab_06b", "B", nullptr);
        perror("execlp B failed");
        return 1;
    }

    // main поток в текущей консоли
    RunLoop("main");

    // Ждём завершения дочерних процессов
    waitpid(pidA, nullptr, 0);
    waitpid(pidB, nullptr, 0);

    // Закрываем и удаляем семафор
    sem_close(g_sem);
    sem_unlink("/Lab_06bSem");

    return 0;
}
