#pragma once
#include "ErrorHandler.h"
#include <time.h>
#include <iostream>
#include <string>
#pragma warning(disable : 4996)


struct Contact
{
	enum TE
	{
		EMPTY,
		ACCEPT,
		CONTACT
	}    type;
	enum ST
	{
		WORK,
		ABORT,
		TIMEOUT,
		FINISH
	}      sthread;

	SOCKET      s;
	SOCKADDR_IN prms;
	int         lprms;
	HANDLE      hthread;
	HANDLE      htimer;
	HANDLE		serverHThtead;

	char msg[50];
	char srvname[15];

	Contact(TE t = EMPTY, const char* namesrv = "")
	{
		memset(&prms, 0, sizeof(SOCKADDR_IN));
		lprms = sizeof(SOCKADDR_IN);
		type = t;
		strcpy(srvname, namesrv);
		msg[0] = 0;
	};

	void SetST(ST sth, const char* m = "")
	{
		sthread = sth;
		strcpy(msg, m);
	}
};

string runServer;

const std::string currentDateTime()
{
	time_t now = time(0);
	tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	return buf;
}

void CALLBACK ASStartMessage(ULONG_PTR p)
{
	printf("RunServer: %s  \nStart time: %s\n", runServer.c_str(), currentDateTime().c_str());

}

void CALLBACK ASFinishMessage(ULONG_PTR p)
{

	printf("\nFinish time: %s\n", currentDateTime().c_str());
}

void QueueUserAPCWrapper(PAPCFUNC functionName, Contact* contact)
{
	QueueUserAPC(functionName, contact->hthread, 0);
}

void SendMsgToClient(Contact* contact)
{
	if (send(contact->s, contact->msg, sizeof(contact->msg), NULL) == SOCKET_ERROR)
		throw  SetErrorMsgText("send:", WSAGetLastError());
}

void SendNumbersToClient(Contact* contact)
{
	if (send(contact->s, contact->msg, strlen(contact->msg), NULL) == SOCKET_ERROR)
		throw  SetErrorMsgText("send:", WSAGetLastError());
}


DWORD WINAPI EchoServer(LPVOID pPrm)
{
	DWORD rc = 0;
	Contact* contact = (Contact*)(pPrm);
	u_long nonblk = 0;
	try
	{
		runServer = "EchoServer";
		QueueUserAPCWrapper(ASStartMessage, contact);
		int libuf;
		contact->sthread = contact->WORK;
		contact->type = contact->CONTACT;
		strcpy(contact->msg, "start transmission");
		SendMsgToClient(contact);


		if (ioctlsocket(contact->s, FIONBIO, &nonblk) == SOCKET_ERROR)
			throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

		while (true)
		{

			if ((libuf = recv(contact->s, contact->msg, sizeof(contact->msg), NULL)) == SOCKET_ERROR)
				throw  SetErrorMsgText("recv:", WSAGetLastError());
			SendMsgToClient(contact);
			Sleep(1000);

			if (strcmp(contact->msg, "exit") == 0) {
				break;
			}
		}
	}
	catch (...)
	{

		//puts("error in dll");
		contact->sthread = contact->ABORT;
		contact->type = contact->EMPTY;
		rc = contact->sthread;

		QueueUserAPCWrapper(ASFinishMessage, contact);
		CancelWaitableTimer(contact->htimer);
		ExitThread(rc);

	}

	contact->type = contact->EMPTY;
	contact->sthread = contact->FINISH;

	nonblk = 1;
	if (ioctlsocket(contact->s, FIONBIO, &nonblk) == SOCKET_ERROR)
		throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

	QueueUserAPCWrapper(ASFinishMessage, contact);
	CancelWaitableTimer(contact->htimer);
	ExitThread(rc);

}

DWORD WINAPI TimeServer(LPVOID pPrm)
{
	DWORD rc = 0;
	Contact* contact = (Contact*)pPrm;
	u_long nonblk = 0;

	try
	{
		runServer = "TimeServer";
		QueueUserAPCWrapper(ASStartMessage, contact);

		contact->sthread = contact->WORK;
		contact->type = contact->CONTACT;

		strcpy(contact->msg, "start time transmission\n");
		SendNumbersToClient(contact);

		if (ioctlsocket(contact->s, FIONBIO, &nonblk) == SOCKET_ERROR)
			throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

		while (true)
		{
			string t = currentDateTime() + "\n";
			strcpy(contact->msg, t.c_str());

			SendNumbersToClient(contact);


			Sleep(1000);
		}
	}
	catch (...)
	{
		//puts("error in TimeServer");

		contact->sthread = contact->ABORT;
		contact->type = contact->EMPTY;
		rc = contact->sthread;

		QueueUserAPCWrapper(ASFinishMessage, contact);
		CancelWaitableTimer(contact->htimer);
		ExitThread(rc);
	}

	contact->type = contact->EMPTY;
	contact->sthread = contact->FINISH;

	nonblk = 1;
	if (ioctlsocket(contact->s, FIONBIO, &nonblk) == SOCKET_ERROR)
		throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

	QueueUserAPCWrapper(ASFinishMessage, contact);
	CancelWaitableTimer(contact->htimer);
	ExitThread(rc);
}

DWORD WINAPI RandomServer(LPVOID pPrm)
{
	DWORD rc = 0;
	Contact* contact = (Contact*)pPrm;
	u_long nonblk = 0;

	try
	{
		runServer = "RandomServer";
		QueueUserAPCWrapper(ASStartMessage, contact);

		contact->sthread = contact->WORK;
		contact->type = contact->CONTACT;

		strcpy(contact->msg, "start random transmission");
		SendNumbersToClient(contact);

		if (ioctlsocket(contact->s, FIONBIO, &nonblk) == SOCKET_ERROR)
			throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

		srand((unsigned int)time(NULL));

		while (true)
		{
			int rnd = rand() % 100 + 1;

			sprintf(contact->msg, "%d", rnd);
			SendNumbersToClient(contact);

			Sleep(1000);
		}
	}
	catch (...)
	{
		//puts("error in RandomServer");

		contact->sthread = contact->ABORT;
		contact->type = contact->EMPTY;
		rc = contact->sthread;

		QueueUserAPCWrapper(ASFinishMessage, contact);
		CancelWaitableTimer(contact->htimer);
		ExitThread(rc);
	}

	contact->type = contact->EMPTY;
	contact->sthread = contact->FINISH;

	nonblk = 1;
	if (ioctlsocket(contact->s, FIONBIO, &nonblk) == SOCKET_ERROR)
		throw SetErrorMsgText("ioctlsocket:", WSAGetLastError());

	QueueUserAPCWrapper(ASFinishMessage, contact);
	CancelWaitableTimer(contact->htimer);
	ExitThread(rc);
}

