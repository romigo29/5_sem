#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>

pthread_mutex_t g_mutex;
std::wstring g_letters;

struct ThreadArg {
    std::wstring name;
};

void* ThreadProc(void* arg) {
    ThreadArg* t = static_cast<ThreadArg*>(arg);
    const std::wstring& threadName = t->name;

    for (int i = 1; i <= 90; ++i) {
        if (i == 30)
            pthread_mutex_lock(&g_mutex);

        wchar_t ch = g_letters[(i - 1) % g_letters.size()];
        std::wcout << threadName << L" – " << i << L" – " << ch << L'\n';

        if (i == 60)
            pthread_mutex_unlock(&g_mutex);

        usleep(100000); // 100 ms
    }

    return nullptr;
}

int main() {
    setlocale(LC_ALL, "");

    // Получаем имя пользователя напрямую через getenv
    const char* user = std::getenv("USER");
    if (!user) user = "UNKNOWN_PC";

    // Преобразуем C-строку в wstring через цикл
    g_letters.clear();
    for (const char* p = user; *p != '\0'; ++p) {
        g_letters.push_back(static_cast<wchar_t>(*p));
    }

    pthread_mutex_init(&g_mutex, nullptr);

    pthread_t tA, tB;

    ThreadArg argA{L"A"};
    ThreadArg argB{L"B"};

    pthread_create(&tA, nullptr, ThreadProc, &argA);
    pthread_create(&tB, nullptr, ThreadProc, &argB);

    for (int i = 1; i <= 90; ++i) {
        if (i == 30)
            pthread_mutex_lock(&g_mutex);

        wchar_t ch = g_letters[(i - 1) % g_letters.size()];
        std::wcout << L"main – " << i << L" – " << ch << L'\n';

        if (i == 60)
            pthread_mutex_unlock(&g_mutex);

        usleep(100000); // 100 ms
    }

    pthread_join(tA, nullptr);
    pthread_join(tB, nullptr);

    pthread_mutex_destroy(&g_mutex);

    return 0;
}
