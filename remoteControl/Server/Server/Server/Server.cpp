#pragma once
//#include <windows.h>

#include <Ws2tcpip.h>
#include "protocol.h"
#include "CMD.h"
#include "SCREEN.h"
#include "FILE.h"

#include <stdio.h>


#pragma comment(lib, "Ws2_32.lib")

CCMD* g_cmd;
CSCREEN* g_screen;
CFILE* g_file;
SOCKET g_socket;
SOCKET g_clentSocket;
bool g_bSendScreen = false;


void reportError(const TCHAR* pre)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR)&lpMsgBuf,
        0,
        NULL
    );

    printf("%s:%s", pre, (char*)lpMsgBuf);


    LocalFree(lpMsgBuf);

}


DWORD WINAPI SendScreenThread(LPVOID lpParameter)
{
    while (true)
    {
        if (!g_bSendScreen)//����ͻ�����˳��������������߳�
        {
            return 0;
        }
        Sleep(500);
        if (!g_bSendScreen)//����ͻ�����˳��������������߳�
        {
            return 0;
        }

        //��������
        g_screen->screenSnap();

    }



}

DWORD WINAPI WorkThread(LPVOID lpParameter)
{
    while (true)
    {
        HEADER head;
        int result = recv(g_clentSocket, (char*)&head, sizeof(head), 0);
        if (result < 0)
        {
            reportError("recv");
            break;
        }


        switch (head.nCmd)
        {
        case CMD_ORDER:
        {
            char szBuff[0x1000] = { 0 };
            int nResult = recv(g_clentSocket, szBuff, head.nLen, 0);
            if (nResult < 0)
            {
                reportError("recv");
                break;
            }
            szBuff[strlen(szBuff)] = '\n';
            g_cmd->execute(szBuff);
            break;
        }
        case SCREEN_CTRL:
        {
            g_bSendScreen = true;

            //�����̣߳�ÿ1�뷢��һ��
            DWORD dwThreadId = 0;
            HANDLE hThread = CreateThread(
                NULL,
                0,
                (LPTHREAD_START_ROUTINE)SendScreenThread,
                (LPVOID)NULL,
                0, //��������
                &dwThreadId);
            if (hThread == NULL)
            {
                MessageBox(NULL, TEXT("create SendScreenThread fail!"), NULL, MB_OK);
            }

            break;
        }
        case SCREEN_OVER:
        {
            g_bSendScreen = false;
            break;
        }
        case SCREEN_COMMAND:
        {
            MSG msg;
            int nResult = recv(g_clentSocket, (char*)&msg, head.nLen, 0);
            if (nResult < 0)
            {
                reportError("recv");
                break;
            }

            //��������
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MOUSEMOVE:
            {
                POINT point;
                point.x = LOWORD(msg.lParam);
                point.y = HIWORD(msg.lParam);
                SetCursorPos(point.x, point.y);
                SetCapture(WindowFromPoint(point));
            }
            default:break;
            }

            //������Ϣ
            switch (msg.message)
            {
            case WM_LBUTTONDOWN:
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                break;
            case WM_LBUTTONUP:
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                break;
            case WM_RBUTTONDOWN:
                mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
                break;
            case WM_RBUTTONUP:
                mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
                break;
            case WM_KEYDOWN:
                keybd_event(msg.wParam, MapVirtualKey(msg.wParam, 0), 0, 0);
                break;
            case WM_KEYUP:
                keybd_event(msg.wParam, MapVirtualKey(msg.wParam, 0), KEYEVENTF_KEYUP, 0);
                break;
            default: break;
            }
            break;
        }
        case FILE_CHECK:
        {
            //��ȡ·��
            char* pPath = new char[head.nLen + 1];
            recv(g_clentSocket, pPath, head.nLen, 0);
            pPath[head.nLen] = '\0';

            g_file->checkFile(pPath);

            delete[] pPath;
            break;
        }
        case FILE_DOWNLOAD:
        {
            g_file->downLoad(head.nLen);
            break;
        }
        case FILE_UPLOAD:
        {
            g_file->upLoad(head.nLen);
            break;
        }
        default: break;
        }

    }
    
    
    return 0;
}


int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    g_socket = s;
    //�󶨶˿�
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1152);
    /************************************************************************************/
    if (inet_pton(AF_INET, "192.168.72.131", &addr.sin_addr) < 0) 
    {
        reportError(TEXT("inet_pton"));
        return 0;
    }
   
    //��
    if (bind(g_socket, (sockaddr*)&addr, sizeof(addr)) < 0)
    {
        reportError(TEXT("bind"));
        return 0;
    }
    else
    {
        printf("bind OK\n");
    }

    //����
    if (listen(g_socket, SOMAXCONN) < 0)
    {
        reportError(TEXT("listen"));
        return 0;
    }
    else
    {
        printf("listen OK\n");
    }

    //��������
    sockaddr_in caddr;
    int len = sizeof(caddr);
    g_clentSocket = accept(g_socket, (sockaddr*)&caddr, &len);
    if (g_clentSocket < 0)
    {
        reportError(TEXT("accept"));
        return 0;
    }
    else
    {
        printf("accept OK\n");
    }


    //cmd��
    CCMD cCmd(g_clentSocket);
    g_cmd = &cCmd;

    //Screen��
    CSCREEN cscreen(g_clentSocket);
    g_screen = &cscreen;

    //File��
    CFILE cfile(g_clentSocket);
    g_file = &cfile;

    //�����߳̽�����Ϣ
    DWORD dwThreadId = 0;
    HANDLE hThread = CreateThread(
        NULL,
        0, 
        (LPTHREAD_START_ROUTINE)WorkThread,
        (LPVOID)NULL,
        0, //��������
        &dwThreadId);
    if (hThread == NULL)
    {
        MessageBox(NULL, TEXT("create WorkThread fail!"), NULL, MB_OK);
    }


    getchar();
    closesocket(g_socket);
    WSACleanup();
    return 0;
}