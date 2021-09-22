#include "FILE.h"
#include <stdio.h>


CFILE::CFILE(SOCKET s)
{
    m_socket = s;
}


CFILE::~CFILE()
{
}

void CFILE::checkFile(char* path)
{
    char szPath[MAX_PATH] = { 0 };
    strcpy_s(szPath, MAX_PATH, path);
    strcat_s(szPath, MAX_PATH, "\\*.*");

    //获取检索文件信息
    WIN32_FIND_DATA wData;
    HANDLE hFile = FindFirstFile(szPath, &wData);

    char szBuff[0x10000] = { 0 };
    strcpy_s(szBuff, 0x10000, "目标路径的文件信息为：\r\n");

    while (FindNextFile(hFile, &wData))
    {
        if (wData.cFileName[0] == '.')//过滤.和..
            continue;

        char szTemp[MAX_PATH] = { 0 };
        strcpy_s(szTemp, MAX_PATH, wData.cFileName);
        int nSize = strlen(szTemp);
        szTemp[nSize] = '\r';
        szTemp[nSize + 1] = '\n';
        szTemp[nSize + 2] = '\0';

        strcat_s(szBuff, 0x10000, szTemp);
    }
    int n = GetLastError();

    FindClose(hFile);
    //回传信息
    HEADER head;
    head.nCmd = FILE_CHECK_INFO;
    head.nLen = strlen(szBuff) + 1 ;

    send(m_socket, (char*)&head, sizeof(head), 0);

    send(m_socket, szBuff, strlen(szBuff) + 1, 0);

}

void CFILE::upLoad(int len)
{
    //接收文件名
    FILENAME fileName;
    recv(m_socket, (char*)&fileName, sizeof(fileName), 0);

    //接收文件信息
    char* pBuff = new char[len];
    int nRecv = 0;
    int bytes = 0;
    while (nRecv < len)
    {
        bytes = recv(m_socket, pBuff + nRecv, len - nRecv, 0);
        nRecv += bytes;
    }
    
    //打开文件
    char szFilePath[MAX_PATH] = { 0 };
    strcpy_s(szFilePath, MAX_PATH, ".\\");
    strcpy_s(szFilePath, MAX_PATH, fileName.szFileName);

    HANDLE hFileDst = CreateFile(
        szFilePath,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFileDst == INVALID_HANDLE_VALUE)
    {
        printf("目标文件打开失败\n");
        return;
    }

    //写入数据
    DWORD dwBytesToWrite = 0;
    if (!WriteFile(hFileDst, pBuff, len, &dwBytesToWrite, NULL))
    {
        printf("写文件出错\n");
        return;
    }
    if (len != dwBytesToWrite)
    {
        printf("文件写入数据丢失\n");
    }

    printf("文件成功上传\n");
    delete[] pBuff;
    CloseHandle(hFileDst);
}

void CFILE::downLoad(int len)
{
    //获取路径
    char szPath[MAX_PATH] = { 0 };
    recv(m_socket, szPath, len, 0);

    HANDLE hFileDst = CreateFile(
        szPath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFileDst == INVALID_HANDLE_VALUE)
    {
        printf("目标文件打开失败\n");
        return;
    }

    //获取文件大小
    DWORD dwFileSize = GetFileSize(hFileDst, NULL);

    //获取文件名
    int nLen = strlen(szPath);
    int i = nLen - 1;
    while (true)
    {
        if (szPath[i] == '\\')
        {
            break;
        }
        i--;
    }
    char szFileName[MAX_PATH] = { 0 };//文件名

    int k = 0;
    int j = i + 1;
    for (; j < nLen; k++, j++)
    {
        szFileName[k] = szPath[j];
    }

    //发送头
    HEADER head;
    head.nCmd = FILE_DOWNLOAD_INFO;
    head.nLen = dwFileSize;
    send(m_socket, (char*)&head, sizeof(head), 0);

    //发送文件名
    FILENAME fName;
    strcpy_s(fName.szFileName, sizeof(fName.szFileName), szFileName);
    send(m_socket, (char*)&fName, sizeof(fName), 0);

    //读取文件并发送
    while (true)
    {
        char szBuff[0x1000] = { 0 };

        //读取数据
        DWORD dwBytesToRead = 0;
        if (!ReadFile(hFileDst, szBuff, sizeof(szBuff), &dwBytesToRead, NULL))
        {
            printf("读取文件失败\n");
            return;
        }

        //如果没有数据,则退出
        if (dwBytesToRead <= 0)
        {
            break;
        }

        //发送数据
        send(m_socket, szBuff, dwBytesToRead, 0);
    }
}
