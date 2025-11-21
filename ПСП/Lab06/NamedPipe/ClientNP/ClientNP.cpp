#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

#define MAX_SIZE_OF_BUFFER 512

string GetErrorMsgText(int code);
string SetPipeError(string msgText, int code);

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    HANDLE cH; // дескриптор канала
    DWORD dwWrite;
    char buffer[50] = "start";

    try
    {
        string serverName;
        cout << "Введите имя сервера: ";
        cin >> serverName;

        // сетевой формат имени канала
        string pipeFullName = "\\\\" + serverName + "\\pipe\\Tube";

        // CreateFile для подключения к серверу по сети
        if ((cH = CreateFileA(
            pipeFullName.c_str(),       // Имя канала
            GENERIC_READ | GENERIC_WRITE, // Чтение и запись
            0,                           // Нет совместного доступа
            NULL,                        // Без атрибутов безопасности
            OPEN_EXISTING,               // Открытие существующего канала
            0,
            NULL)) == INVALID_HANDLE_VALUE)
        {
            throw SetPipeError("CreateFile: ", GetLastError());
        }

        int countOfMessages;
        cout << "Введите кол-во сообщений: ";
        cin >> countOfMessages;
        cin.ignore();

        cout << "Введите сообщение: ";
        cin.getline(buffer, sizeof(buffer));

        for (int i = 1; i <= countOfMessages; i++)
        {
      

            if (!WriteFile(cH, buffer, strlen(buffer), &dwWrite, NULL))
            {
                throw SetPipeError("WriteFile: ", GetLastError());
            }
            cout << "Отправил серверу СМС: " << buffer << endl;

            DWORD dwRead;
            if (!ReadFile(cH, buffer, MAX_SIZE_OF_BUFFER, &dwRead, NULL))
            {
                cout << GetLastError();
                throw SetPipeError("ReadFile: ", GetLastError());
            }
            buffer[dwRead] = '\0';
            cout << "Сервер прислал СМС: " << buffer << endl;
            cout << "----------------------------" << endl;
        }

        CloseHandle(cH);
    }
    catch (string ErrorPipeText)
    {
        cout << "\nError in ClientNP: " << ErrorPipeText;
    }

    return 0;
}

// Функции обработки ошибок (как у тебя)
string GetErrorMsgText(int code)
{
    switch (code)
    {
    case ERROR_PIPE_BUSY: return "Все экземпляры канала заняты";
    case ERROR_FILE_NOT_FOUND: return "Сервер не найден";
    case ERROR_ACCESS_DENIED: return "Доступ запрещен";
    case ERROR_PIPE_CONNECTED: return "Есть процесс на другом конце канала";
    default: return "**ERROR**";
    }
}

string SetPipeError(string msgText, int code)
{
    return msgText + GetErrorMsgText(code) + "\n";
}
