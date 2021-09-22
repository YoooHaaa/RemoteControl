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
    void execute(char* pOrder);//创建一个线程持续读取数据并发送

    HANDLE m_hReadResult;
    HANDLE m_hWriteOrder;
    HANDLE m_hReadOrder;
    HANDLE m_hWriteResult;

    SOCKET m_cs;
};

