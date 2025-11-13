#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <ws2tcpip.h>
#include <tchar.h>
#include "ErrorHandler.h"
#pragma comment(lib,"WS2_32.lib")

using namespace std;

SOCKET sS;
SOCKET cS;


bool GetServer(char* call, short port, sockaddr* from, int* flen)
{
	SOCKADDR_IN all;
	all.sin_addr.s_addr = INADDR_BROADCAST;
	all.sin_family = AF_INET;
	all.sin_port = htons(2000);
	from = (sockaddr*)&all;
	int err = 0;

	if ((err = sendto(cS, call, strlen(call) + 1, NULL, from, *flen) == SOCKET_ERROR))
	{
		throw SetErrorMsgText("sendto:", WSAGetLastError());
	}

	if ((err = recvfrom(cS, call, sizeof(call), NULL, from, flen) == SOCKET_ERROR))
	{
		return false;
	}
	cout << call << endl;
	return true;

}
bool GetRequestFromClient(char* name, short port, sockaddr* from, int* flen)
{
	char bfrom[50];
	int err = 0;
	while (true)
	{

		if ((err = recvfrom(sS, bfrom, sizeof(bfrom), NULL, from, flen)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("rexevfrom: ", WSAGetLastError());
		}
		if (strcmp(name, bfrom) == 0)
		{
			return true;
		}
	}

}

bool PutAnswerToClient(char* name, sockaddr* to, int* lto)
{
	int err = 0;
	if ((err = sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
	{
		throw SetErrorMsgText("sendto: ", WSAGetLastError());
		return false;
	}
	return true;
}


void main()
{
	try
	{

		setlocale(LC_ALL, "rus");
		char name[10] = "Hello";
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw SetErrorMsgText("wsaSartUp: ", WSAGetLastError());
		}

		if ((cS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("socket create: ", WSAGetLastError());
		}

		DWORD timeout = 10;
		if (setsockopt(cS, SOL_SOCKET, SO_BROADCAST, (char*)&timeout, sizeof(int)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("setsockopt: ", WSAGetLastError());
		}

		int optval = 1;
		if (setsockopt(cS, SOL_SOCKET, SO_RCVTIMEO, (char*)&optval, sizeof(int)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("setsockopt: ", WSAGetLastError());
		}

		SOCKADDR_IN all;
		int allLen = sizeof(all);
		if (GetServer(name, 2000, (sockaddr*)&all, &allLen))
		{
			cout << "В сети есть несколько серверов с позывным " << name << endl;
		}
		else
		{
			cout << "В сети один сервер с позывным " << name << endl;
		}
		if (closesocket(cS) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("close socket: ", WSAGetLastError());
		}

		if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
		{
			throw SetErrorMsgText("creaate socket: ", WSAGetLastError());
		}

		SOCKADDR_IN  serv;
		serv.sin_addr.s_addr = INADDR_ANY;
		serv.sin_family = AF_INET;
		serv.sin_port = htons(2000);
		BOOL optval2 = TRUE;
		setsockopt(sS, SOL_SOCKET, SO_REUSEADDR, (char*)&optval2, sizeof(optval));

		if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("bind: ", WSAGetLastError());
		}



		SOCKADDR_IN  from;
		memset(&from, 0, sizeof(from));
		int lbfrom = sizeof(from);
		char hostName[20];
		int err = 0;
		hostent* addr;

		if ((err = gethostname(hostName, sizeof(hostName))) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("get host name: ", WSAGetLastError());
		}

		cout << "host name: " << hostName;
		while (true)
		{
			if (!GetRequestFromClient(name, 2000, (sockaddr*)&from, &lbfrom))
			{
				throw SetErrorMsgText("getRequestFromClient: ", WSAGetLastError());
			}

			std::cout << endl << "IP: " << inet_ntoa(from.sin_addr) << "\tport: " << htons(from.sin_port) << endl;
			lbfrom = sizeof(from);

			struct in_addr ip_addr = from.sin_addr;
			struct hostent* addr = gethostbyaddr((const char*)&ip_addr, sizeof(ip_addr), AF_INET);

			if (addr != NULL) {
				cout << "host name: " << addr->h_name << endl;
			}

			if (!PutAnswerToClient(name, (sockaddr*)&from, &lbfrom))
			{
				throw SetErrorMsgText("put answer: ", WSAGetLastError());
			}
		}


		if (closesocket(sS) == SOCKET_ERROR)
		{
			throw SetErrorMsgText("close socket: ", WSAGetLastError());
		}
		if (WSACleanup() == SOCKET_ERROR)
		{
			throw SetErrorMsgText("cleanup: ", WSAGetLastError());
		}
	}
	catch (string errorMsgText) {
		cout << endl << "WSAGetLastError: " << errorMsgText;
	}

}
