#include "stdafx.h"

#define NAME L"\\\\.\\mailslot\\Box"
int main()
{
	setlocale(LC_ALL, "rus");
	cout << "Mailslot Client" << endl;

	HANDLE hM;     
	DWORD wb;  
	clock_t start, end;

	char obuf[50];
	try
	{
		if ((hM = CreateFile(
			NAME,
			GENERIC_WRITE,     
			FILE_SHARE_READ,     
			NULL,
			OPEN_EXISTING,    
			NULL,
			NULL
		)) == INVALID_HANDLE_VALUE)
			throw SetErrorMsgText("CreateFileError", WSAGetLastError());

		start = clock();
		for (int i = 0; i < 1000; i++)
		{

			string obufstr = "hello " + to_string(i + 1);
			strcpy_s(obuf, obufstr.c_str());

			if (!WriteFile(
				hM,
				obuf,  
				sizeof(obuf),   
				&wb,  
				NULL
			))
				throw SetErrorMsgText("ReadFileError", WSAGetLastError());

			cout << obuf << endl;
		}
		end = clock();
		cout << "\nВремя передачи: " << ((double)(end - start) / CLK_TCK) << " секунд" << endl;

		CloseHandle(hM);

	}
	catch (string error)
	{
		cout << endl << error << endl;
	}
}
