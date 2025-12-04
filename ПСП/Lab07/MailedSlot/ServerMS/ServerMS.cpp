#include "stdafx.h"

#define NAME L"\\\\.\\mailslot\\Box"

int main()
{
    setlocale(LC_ALL, "rus");
    cout << "Mailslot Server" << endl;

    HANDLE hM;     
    DWORD rb;    
    char rbuf[300];   
    DWORD time_waiting = 180000;
    clock_t start, end;

    try
    {
        if ((hM = CreateMailslot(
            NAME,
            500,
            time_waiting,   
            NULL
        )) == INVALID_HANDLE_VALUE)
            throw SetErrorMsgText("CreateMailslotError", WSAGetLastError());

        while (true)
        {
            if (ReadFile(
                hM,
                rbuf,  
                sizeof(rbuf),   
                &rb,  
                NULL
            ))
            {
                cout << rbuf << endl;
            }
            else
                throw SetErrorMsgText("ReadFileError", WSAGetLastError());

        }

        CloseHandle(hM);
    }
    catch (string error)
    {
        cout << endl << error << endl;
    }
}


