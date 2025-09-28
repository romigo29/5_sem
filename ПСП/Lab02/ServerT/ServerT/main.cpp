#include "Winsock2.h" // заголовок WS2_32.dll
#include <Windows.h>
#include <iostream>
#include <string>
#include <TCHAR.h>
using namespace std;
#pragma comment(lib, "WS2_32.lib") // экспорт WS2_32.dll
#pragma warning (disable: 4996)

#define PORT 2000

string GetErrorMsgText(int code) // cформировать текст ошибки
{
    string msgText;
    switch (code)                      // проверка кода возврата  
    {
    case WSAEINTR: msgText = "Работа функции прервана "; break;
    case WSAEACCES: msgText = "Разрешение отвергнуто"; break;
    case WSAEFAULT: msgText = "Ошибочный адрес"; break;
    case WSAEINVAL: msgText = "Ошибка в аргументе "; break;
    case WSAEMFILE: msgText = "Слишком много файлов открыто"; break;
    case WSAEWOULDBLOCK: msgText = "Ресурс временно недоступен"; break;
    case WSAEINPROGRESS: msgText = "Операция в процессе развития"; break;
    case WSAEALREADY: msgText = "Операция уже выполняется "; break;
    case WSAENOTSOCK: msgText = "Сокет задан неправильно   "; break;
    case WSAEDESTADDRREQ: msgText = "Требуется адрес расположения "; break;
    case WSAEMSGSIZE: msgText = "Сообщение слишком длинное "; break;
    case WSAEPROTOTYPE: msgText = "Неправильный тип протокола для сокета "; break;
    case WSAENOPROTOOPT: msgText = "Ошибка в опции протокола"; break;
    case WSAEPROTONOSUPPORT: msgText = "Протокол не поддерживается "; break;
    case WSAESOCKTNOSUPPORT: msgText = "Тип сокета не поддерживается "; break;
    case WSAEOPNOTSUPP: msgText = "Операция не поддерживается "; break;
    case WSAEPFNOSUPPORT: msgText = "Тип протоколов не поддерживается "; break;
    case WSAEAFNOSUPPORT: msgText = "Тип адресов не поддерживается протоколом"; break;
    case WSAEADDRINUSE: msgText = "Адрес уже используется "; break;
    case WSAEADDRNOTAVAIL: msgText = "Запрошенный адрес не может быть использован"; break;
    case WSAENETDOWN: msgText = "Сеть отключена "; break;
    case WSAENETUNREACH: msgText = "Сеть не достижима"; break;
    case WSAENETRESET: msgText = "Сеть разорвала соединение"; break;
    case WSAECONNABORTED: msgText = "Программный отказ связи "; break;
    case WSAECONNRESET: msgText = "Связь восстановлена "; break;
    case WSAENOBUFS: msgText = "Не хватает памяти для буферов"; break;
    case WSAEISCONN: msgText = "Сокет уже подключен"; break;
    case WSAENOTCONN: msgText = "Сокет не подключен"; break;
    case WSAESHUTDOWN: msgText = "Нельзя выполнить send: сокет завершил работу"; break;
    case WSAETIMEDOUT: msgText = "Закончился отведенный интервал  времени"; break;
    case WSAECONNREFUSED: msgText = "Соединение отклонено  "; break;
    case WSAEHOSTDOWN: msgText = "Хост в неработоспособном состоянии"; break;
    case WSAEHOSTUNREACH: msgText = "Нет маршрута для хоста "; break;
    case WSAEPROCLIM: msgText = "Слишком много процессов "; break;
    case WSASYSNOTREADY: msgText = "Сеть не доступна "; break;
    case WSAVERNOTSUPPORTED: msgText = "Данная версия недоступна "; break;
    case WSANOTINITIALISED: msgText = "Не выполнена инициализация WS2_32.DLL"; break;
    case WSAEDISCON: msgText = "Выполняется отключение"; break;
    case WSATYPE_NOT_FOUND: msgText = "Класс не найден "; break;
    case WSAHOST_NOT_FOUND: msgText = "Хост не найден"; break;
    case WSATRY_AGAIN: msgText = "Неавторизированный хост не найден "; break;
    case WSANO_RECOVERY: msgText = "Неопределенная  ошибка "; break;
    case WSANO_DATA: msgText = "Нет записи запрошенного типа "; break;
    case WSA_INVALID_HANDLE: msgText = "Указанный дескриптор события  с ошибкой   "; break;
    case WSA_INVALID_PARAMETER: msgText = "Один или более параметров с ошибкой   "; break;
    case WSA_IO_INCOMPLETE: msgText = "Объект ввода-вывода не в сигнальном состоянии"; break;
    case WSA_IO_PENDING: msgText = "Операция завершится позже  "; break;
    case WSA_NOT_ENOUGH_MEMORY: msgText = "Не достаточно памяти "; break;
    case WSA_OPERATION_ABORTED: msgText = "Операция отвергнута "; break;
    case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова "; break;
    default:                msgText = "***ERROR***";      break;
    };
    return msgText;
};

string SetErrorMsgText(string msgText, int code)
{
	return msgText + GetErrorMsgText(code);
};

int main(int argc, _TCHAR* argv[])
{

    setlocale(LC_ALL, "RUS");
    WSADATA wsaData;
    //............................................................
    try
    { 
        while (true) 
        {
            if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
                throw SetErrorMsgText("Startup:", WSAGetLastError());
            //............................................................
            SOCKET sS; //сетевой сокет
            if ((sS = socket(AF_INET, SOCK_STREAM, NULL)) == INVALID_SOCKET)
                throw SetErrorMsgText("socket:", WSAGetLastError());

            SOCKADDR_IN serv; // параметры сокета sS
            serv.sin_family = AF_INET; // используется IP-адресация 
            serv.sin_port = htons(PORT); // порт 2000
            serv.sin_addr.s_addr = inet_addr("127.0.0.1");

            if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR) // установка параметров сокета
                throw SetErrorMsgText("bind:", WSAGetLastError());
            if (listen(sS, SOMAXCONN) == SOCKET_ERROR) //режим прослушивания
                throw SetErrorMsgText("listen:", WSAGetLastError());

            SOCKET cS; // сокет для обмена данными с клиентом
            SOCKADDR_IN clnt; // параметры сокета клиента
            memset(&clnt, 0, sizeof(clnt)); // обнулить память
            int lclnt = sizeof(clnt); // размер SOCKADDR_IN

            if ((cS = accept(sS, (sockaddr*)&clnt, &lclnt)) == INVALID_SOCKET)
                throw SetErrorMsgText("accept:", WSAGetLastError());

            cout << "Client connected: IP = " << inet_ntoa(clnt.sin_addr) << ", Port = " << htons(clnt.sin_port) << endl;

            char ibuf[50], //буфер ввода
                obuf[50] = "sever: принято "; //буфер вывода
            int libuf = 0, //количество принятых байт
                lobuf = 0; //количество отправленных байь

            while (true) {
                if ((libuf = recv(cS, ibuf, sizeof(ibuf), NULL)) == SOCKET_ERROR)
                    throw SetErrorMsgText("recv:", WSAGetLastError());
                _itoa(lobuf, obuf + sizeof("sever: принято ") - 1, 10);

                cout << ibuf << endl;

                if ((lobuf = send(cS, obuf, strlen(obuf) + 1, NULL)) == SOCKET_ERROR)
                    throw SetErrorMsgText("send:", WSAGetLastError());
            }
            //.............................................................
            if (closesocket(sS) == SOCKET_ERROR)
                throw SetErrorMsgText("closesocket:", WSAGetLastError());
            if (WSACleanup() == SOCKET_ERROR)
                throw SetErrorMsgText("Cleanup:", WSAGetLastError());

            cout << "Lost connection" << endl << "Wait for another connection" << endl;
            system("pause");
        }
	}
	catch (string errorMsgText)
	{
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}
	//................................................................
	return 0;
}