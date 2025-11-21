#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

#define PIPE_NAME L"\\\\.\\pipe\\Tube"
#define MAX_SIZE_OF_BUFFER 512

string GetErrorMsgText(int code)
{
    switch (code)
    {
    case ERROR_PIPE_BUSY: return "Все экземпляры канала заняты";
    case ERROR_FILE_NOT_FOUND: return "Сервер не найден";
    case ERROR_ACCESS_DENIED: return "Доступ запрещен";
    case ERROR_PIPE_CONNECTED: return "Есть процесс на другом конце канала";
    case ERROR_BROKEN_PIPE: return "Клиент разорвал соединение";
    default: return "**UNKNOWN ERROR**";
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

    try
    {
        // Настройка безопасности канала (доступ всем)
        SECURITY_ATTRIBUTES sa;
        SECURITY_DESCRIPTOR sd;

        InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
        SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE); // полный доступ всем
        sa.nLength = sizeof(sa);
        sa.lpSecurityDescriptor = &sd;
        sa.bInheritHandle = FALSE;

        // Создаем именованный канал
        HANDLE sH = CreateNamedPipe(
            PIPE_NAME,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            MAX_SIZE_OF_BUFFER,
            MAX_SIZE_OF_BUFFER,
            0,
            &sa
        );

        if (sH == INVALID_HANDLE_VALUE)
            throw SetPipeError("CreateNamedPipe: ", GetLastError());

        cout << "Сервер запущен. Ожидание клиента..." << endl;

        while (true)
        {
            // Ждем подключения клиента
            if (!ConnectNamedPipe(sH, NULL))
            {
                DWORD err = GetLastError();
                if (err != ERROR_PIPE_CONNECTED)
                    throw SetPipeError("ConnectNamedPipe: ", err);
            }

            cout << "Клиент подключен!" << endl;

            char buffer[MAX_SIZE_OF_BUFFER];
            DWORD dwRead, dwWritten;

            // Чтение и запись сообщений
            while (true)
            {
                BOOL fSuccess = ReadFile(sH, buffer, MAX_SIZE_OF_BUFFER - 1, &dwRead, NULL);
                if (!fSuccess)
                {
                    DWORD err = GetLastError();
                    if (err == ERROR_BROKEN_PIPE)
                    {
                        cout << "Клиент отключился." << endl;
                        break; // выход из внутреннего цикла
                    }
                    else
                        throw SetPipeError("ReadFile: ", err);
                }

                buffer[dwRead] = '\0'; // конец строки
                cout << "Клиент прислал: " << buffer << endl;

                // Отправляем обратно клиенту
                fSuccess = WriteFile(sH, buffer, dwRead, &dwWritten, NULL);
                if (!fSuccess)
                    throw SetPipeError("WriteFile: ", GetLastError());
                cout << "Отправил клиенту: " << buffer << endl;
            }

            // Разрываем соединение
            DisconnectNamedPipe(sH);
            cout << "Ожидание нового клиента..." << endl;
        }

        CloseHandle(sH);
    }
    catch (string ErrorPipeText)
    {
        cout << "\nError in ServerNP: " << ErrorPipeText << endl;
    }

    return 0;
}
