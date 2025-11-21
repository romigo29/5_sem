#include <iostream>
#include <windows.h>
#include <string>

#pragma warning(disable:4996)

using namespace std;

#define PIPE_NAME L"\\\\PCI\\pipe\\Tube" // Локальный канал
#define MAX_SIZE_OF_BUFFER 512

string GetErrorMsgText(int code)
{
    switch (code)
    {
    case ERROR_FILE_NOT_FOUND: return "Сервер не найден";
    case ERROR_ACCESS_DENIED: return "Доступ запрещен";
    case ERROR_PIPE_BUSY: return "Все экземпляры канала заняты";
    case ERROR_NO_DATA: return "Канал закрыт";
    case ERROR_PIPE_CONNECTED: return "Канал уже соединен с другим процессом";
    default: return "Неизвестная ошибка: " + to_string(code);
    }
}

string SetPipeError(string msgText, int code)
{
    return msgText + GetErrorMsgText(code) + "\n";
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char buffer[] = "Hello Server";
    char outbuffer[MAX_SIZE_OF_BUFFER] = { 0 };
    DWORD bytesRead = 0;

    try
    {
        // Подключаемся к серверу
        HANDLE hPipe = CreateFile(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0,              // Нет совместного доступа
            NULL,           // Атрибуты безопасности
            OPEN_EXISTING,  // Открыть существующий канал
            0,
            NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE)
            throw SetPipeError("CreateFile: ", GetLastError());

        // Устанавливаем режим сообщений
        DWORD mode = PIPE_READMODE_MESSAGE;
        if (!SetNamedPipeHandleState(hPipe, &mode, NULL, NULL))
            throw SetPipeError("SetNamedPipeHandleState: ", GetLastError());

        cout << "Отправка серверу: " << buffer << endl;

        // Отправка и получение ответа
        if (!TransactNamedPipe(
            hPipe,
            buffer,
            (DWORD)strlen(buffer),
            outbuffer,
            MAX_SIZE_OF_BUFFER,
            &bytesRead,
            NULL))
        {
            throw SetPipeError("TransactNamedPipe: ", GetLastError());
        }

        outbuffer[bytesRead] = '\0'; // конец строки
        cout << "Сервер ответил: " << outbuffer << endl;

        CloseHandle(hPipe);
    }
    catch (string errorMsg)
    {
        cout << "Ошибка клиента: " << errorMsg << endl;
    }

    return 0;
}
