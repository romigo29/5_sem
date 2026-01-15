#include <windows.h>
#include <iostream>

// Любая функция
void TestFunction()
{
}

// Глобальные переменные
int g_init = 10;          // глобальная инициализированная
int g_uninit;             // глобальная неинициализированная

// Глобальные статические переменные
static int gs_init = 20;  // глобальная статическая инициализированная
static int gs_uninit;     // глобальная статическая неинициализированная

int main(int argc, char* argv[])
{
    // Локальные переменные
    int l_init = 30;      // локальная инициализированная
    int l_uninit;         // локальная неинициализированная

    // Локальные статические переменные
    static int ls_init = 40; // локальная статическая инициализированная
    static int ls_uninit;    // локальная статическая неинициализированная

    std::cout << "Addresses of functions:\n";
    std::cout << "TestFunction: " << (void*)&TestFunction << "\n";
    std::cout << "main:         " << (void*)&main << "\n\n";

    std::cout << "Global variables:\n";
    std::cout << "g_init:       " << (void*)&g_init << "\n";
    std::cout << "g_uninit:     " << (void*)&g_uninit << "\n\n";

    std::cout << "Global static variables:\n";
    std::cout << "gs_init:      " << (void*)&gs_init << "\n";
    std::cout << "gs_uninit:    " << (void*)&gs_uninit << "\n\n";

    std::cout << "Local variables:\n";
    std::cout << "l_init:       " << (void*)&l_init << "\n";
    std::cout << "l_uninit:     " << (void*)&l_uninit << "\n\n";

    std::cout << "Local static variables:\n";
    std::cout << "ls_init:      " << (void*)&ls_init << "\n";
    std::cout << "ls_uninit:    " << (void*)&ls_uninit << "\n\n";

    std::cout << "Arguments of main:\n";
    std::cout << "argc:         " << (void*)&argc << "\n";
    std::cout << "argv:         " << (void*)&argv << "\n\n";

    std::cout << "Press any key to continue...\n";
    std::cin.get();

    return 0;
}
