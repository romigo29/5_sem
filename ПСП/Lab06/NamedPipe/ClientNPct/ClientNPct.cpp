#include <iostream>
#include <windows.h>
#include <string>

#pragma warning(disable:4996)

using namespace std;

#define PIPE_NAME L"\\\\.\\pipe\\Tube"   
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
        cout << "Отправка серверу: " << buffer << endl;

        if (!CallNamedPipe(
            PIPE_NAME,             
            buffer,                
            (DWORD)strlen(buffer),    
            outbuffer,             
            MAX_SIZE_OF_BUFFER,     
            &bytesRead,             
            5000))                  
        {
            throw SetPipeError("CallNamedPipe:", GetLastError());
        }

        outbuffer[bytesRead] = '\0';   
        cout << "Сервер ответил: " << outbuffer << endl;
    }
    catch (string errorMsg)
    {
        cout << "Ошибка клиента: " << errorMsg << endl;
    }

    return 0;
}
