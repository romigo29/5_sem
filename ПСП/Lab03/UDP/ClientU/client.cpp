#include <winsock2.h>
#include <ws2tcpip.h>
#include <tchar.h>
#include <iostream>
#include <string>
#pragma comment(lib,"WS2_32.lib")
using namespace std;
#define SEND_AND_GET

string GetErrorMsgText(int code);
string SetErrorMsgText(string text, int code);

int main(int argc, _TCHAR* argv[]) {

	setlocale(LC_ALL, "ru");

	try {
		//1
		WSADATA WSD_pointer;
		int WSD_version = MAKEWORD(2, 0);

		if (WSAStartup(WSD_version, &WSD_pointer) != 0) {
			throw SetErrorMsgText("Failed to start", WSAGetLastError());
		}
		cout << "--client started" << endl;

		//2

		SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if (clientSocket == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to create a client socket", WSAGetLastError());
		}

		cout << "--client socket created" << endl;

		//3
		SOCKADDR_IN serv;

		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		if (inet_pton(AF_INET, "192.168.48.1", &serv.sin_addr.S_un.S_addr) <= 0) {
			throw SetErrorMsgText("Failed to set an IP address", WSAGetLastError());
		}

		string outBuffer;
		int LoutBuffer = 0;

		outBuffer = "Hello";
		LoutBuffer = sendto(clientSocket, outBuffer.c_str(), outBuffer.size() + 1, NULL, (sockaddr*)&serv, sizeof(serv));

		if (LoutBuffer == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to send message to server", WSAGetLastError());
		}

		cout << "--message sent (" << outBuffer << ")" << endl;

		//4
		char inBuffer[50];
		int LinBuffer = 0;
		SOCKADDR_IN inServ;
		int LinServ = sizeof(inServ);

		LinBuffer = recvfrom(clientSocket, inBuffer, sizeof(inBuffer) - 1, NULL, (sockaddr*)&serv, &LinServ);

		if (LinBuffer == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to receive message from server", WSAGetLastError());
		}

		inBuffer[LinBuffer] = '\0';

		cout << "--received from server (" << inBuffer << ")" << endl;

		int amountMessages;
		cout << "Enter the amount of messages to be sent: ";
		cin >> amountMessages;

		string iNaBuffer;
		int LiNaBuffer = 0;

		for (int i = 0; i < amountMessages; i++) {
			outBuffer = "Hello from ClientU" + to_string(i);
			LoutBuffer = sendto(clientSocket, outBuffer.c_str(), outBuffer.size() + 1, NULL, (sockaddr*)&serv, sizeof(serv));

			if (LoutBuffer == SOCKET_ERROR) {
				throw SetErrorMsgText("Failed to send message to server", WSAGetLastError());
			}

			cout << "--message sent (" << outBuffer << ")" << endl;



			LinBuffer = recvfrom(clientSocket, inBuffer, sizeof(inBuffer) - 1, NULL, (sockaddr*)&serv, &LinServ);

			if (LinBuffer == SOCKET_ERROR) {
				throw SetErrorMsgText("Failed to receive message from server", WSAGetLastError());
			}

			inBuffer[LinBuffer] = '\0';

			cout << "--received from server (" << inBuffer << ")" << endl;

		}


		if (sendto(clientSocket, "", 0, NULL, (sockaddr*)&serv, sizeof(serv)) == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to send the last message to the server", WSAGetLastError());
		}
		cout << "--last message sent to the server. disconnecting." << endl;

		//5
		if (closesocket(clientSocket) == SOCKET_ERROR) {
			throw SetErrorMsgText("Failed to close client socket", WSAGetLastError());
		}
		cout << "--client socket closed" << endl;

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