#pragma once
#include <windows.h>
#include <ctime>

class CCMD
{
public:
    CCMD(int s);
    ~CCMD();

private:
    

    

public:
    void execute(char* pOrder);//����һ���̳߳�����ȡ���ݲ�����

    HANDLE m_hReadResult;
    HANDLE m_hWriteOrder;
    HANDLE m_hReadOrder;
    HANDLE m_hWriteResult;

    SOCKET m_cs;
};

