#include <winsock2.h>
#include <tchar.h>
#include <iostream>
#include <string>
#pragma comment(lib,"WS2_32.lib")
using namespace std;
#define SEND_AND_GET

string GetErrorMsgText(int code);
string SetErrorMsgText(string text, int code);

int main(int argc, _TCHAR* argv[]) {

	try {
		WSADATA WSA_pointer;
		int WSD_Version = MAKEWORD(2, 0);

		if (WSAStartup(WSD_Version, &WSA_pointer) != 0) {
			throw SetErrorMsgText("WSAStartup error", WSAGetLastError());
		}
		cout << "--server started" << endl;

		SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (serverSocket == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to create server socket", WSAGetLastError());
		}

		cout << "--server socket created" << endl;

		SOCKADDR_IN serverParameters;
		serverParameters.sin_family = AF_INET;
		serverParameters.sin_port = htons(2000);
		serverParameters.sin_addr.S_un.S_addr = INADDR_ANY;

		if (bind(serverSocket, (LPSOCKADDR)&serverParameters, sizeof(serverParameters)) == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to bind socket parameters", WSAGetLastError());
		}

		cout << "--parameters binded" << endl;

		SOCKET clientSocket;
		SOCKADDR_IN clientParameters;

		memset(&clientParameters, 0, sizeof(clientParameters));
		int LClient = sizeof(clientParameters);

		char inBuffer[50];
		int LinBuffer = 0;

		cout << "--recvfrom and wait" << endl;

		LinBuffer = recvfrom(serverSocket, inBuffer, sizeof(inBuffer), NULL, (sockaddr*)&clientParameters, &LClient);

		if (LinBuffer == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to receive message from client", WSAGetLastError());
		}
		inBuffer[LinBuffer] = '\0';

		cout << "--got from client (" << inBuffer << ")" << endl;
		cout << "--sending the message back to client" << endl;

		int sendResult = sendto(serverSocket, inBuffer, LinBuffer, NULL, (sockaddr*)&clientParameters, LClient);
		if (sendResult == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to send message back to client", WSAGetLastError());
		}

		cout << "--sent back to client (" << inBuffer << ")" << endl;

		while (true) {

			LinBuffer = recvfrom(serverSocket, inBuffer, sizeof(inBuffer), NULL, (sockaddr*)&clientParameters, &LClient);

			if (LinBuffer == SOCKET_ERROR) {
				throw SetErrorMsgText("Failed to receive message from client", WSAGetLastError());
			}
			if (LinBuffer == 0) {
				cout << "--last message was received. client disconnected." << endl;
				break;
			}
			inBuffer[LinBuffer] = '\0';

			cout << "--got from client (" << inBuffer << ")" << endl;

			Sleep(10); 

		/*	cout << "--sending the message back to client" << endl;


			if (sendto(serverSocket, inBuffer, LinBuffer - 1, NULL, (sockaddr*)&clientParameters, LClient) == SOCKET_ERROR) {
				throw SetErrorMsgText("Failed to send message back to client", WSAGetLastError());
			}

			cout << "--sent back to client (" << inBuffer << ")" << endl;*/

		}


		if (closesocket(serverSocket) == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to close server socket", WSAGetLastError());
		}

		cout << "--server socket closed" << endl;

		if (WSACleanup() == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to cleanup", WSAGetLastError());
		}

		cout << "--cleanup executed" << endl;

		system("pause");


	}
	catch (string message) {
		cout << "An error occured: " << message << endl;
	}


	return 0;
}


string SetErrorMsgText(string text, int code) {
	char code_chr = static_cast<char>(code);
	return text + GetErrorMsgText(code) + "(" + code_chr + ")";
}

string GetErrorMsgText(int code)
{
	string msgText;
	switch (code)
	{
	case WSAEINTR: msgText = "Работа функции прервана"; break;
	case WSAEACCES: msgText = "Разрешение отвергнуто"; break;
	case WSAEFAULT: msgText = "Ошибочный адрес"; break;
	case WSAEINVAL: msgText = "Ошибка в аргументе"; break;
	case WSAEMFILE: msgText = "Открыто слишком много файлов"; break;
	case WSAEWOULDBLOCK: msgText = "Ресурс временно недоступен"; break;
	case WSAEINPROGRESS: msgText = "Операция в процессе"; break;
	case WSAEALREADY: msgText = "Операция уже выполняется"; break;
	case WSAENOTSOCK: msgText = "Сокет задан неправильно"; break;
	case WSAEDESTADDRREQ: msgText = "Требуется адрес расположения"; break;
	case WSAEMSGSIZE: msgText = "Сообщение слишком длинное"; break;
	case WSAEPROTOTYPE: msgText = "Неправильный тип протокола для сокета"; break;
	case WSAENOPROTOOPT: msgText = "Ошибка в опции протокола"; break;
	case WSAEPROTONOSUPPORT: msgText = "Протокол не поддерживается"; break;
	case WSAESOCKTNOSUPPORT: msgText = "Тип сокета не поддерживается"; break;
	case WSAEOPNOTSUPP: msgText = "Операция не поддерживается"; break;
	case WSAEPFNOSUPPORT: msgText = "Тип протоколов не поддерживается"; break;
	case WSAEAFNOSUPPORT: msgText = "Тип адресов не поддерживается протоколом"; break;
	case WSAEADDRINUSE: msgText = "Адрес уже используется"; break;
	case WSAEADDRNOTAVAIL: msgText = "Запрошенный адрес не может быть использован"; break;
	case WSAENETDOWN: msgText = "Сеть отключена"; break;
	case WSAENETUNREACH: msgText = "Сеть не достижима"; break;
	case WSAENETRESET: msgText = "Сеть разорвала соединение"; break;
	case WSAECONNABORTED: msgText = "Программный отказ связи"; break;
	case WSAECONNRESET: msgText = "Связь не восстановлена"; break;
	case WSAENOBUFS: msgText = "Не хватает памяти для буферов"; break;
	case WSAEISCONN: msgText = "Сокет уже подключен"; break;
	case WSAENOTCONN: msgText = "Сокет не подключен"; break;
	case WSAESHUTDOWN: msgText = "Нельзя выполнить send: сокет завершил работу"; break;
	case WSAETIMEDOUT: msgText = "Закончился отведенный интервал времени"; break;
	case WSAECONNREFUSED: msgText = "Соединение отклонено"; break;
	case WSAEHOSTDOWN: msgText = "Хост в неработоспособном состоянии"; break;
	case WSAEHOSTUNREACH: msgText = "Нет маршрута для хоста"; break;
	case WSAEPROCLIM: msgText = "Слишком много процессов"; break;
	case WSASYSNOTREADY: msgText = "Сеть не доступна"; break;
	case WSAVERNOTSUPPORTED: msgText = "Данная версия недоступна"; break;
	case WSANOTINITIALISED: msgText = "Не выполнена инициализация WS2_32.dll"; break;
	case WSAEDISCON: msgText = "Выполняется отключение"; break;
	case WSATYPE_NOT_FOUND: msgText = "Класс не найден"; break;
	case WSAHOST_NOT_FOUND: msgText = "Хост не найден"; break;
	case WSATRY_AGAIN: msgText = "Неавторизованный хост не найден"; break;
	case WSANO_RECOVERY: msgText = "Неопределенная ошибка"; break;
	case WSANO_DATA: msgText = "Нет записи запрошенного типа"; break;
	case WSASYSCALLFAILURE: msgText = "Аварийное завершение системного вызова"; break;
	default: msgText = "Unknown error"; break;
	}
	return msgText;
}