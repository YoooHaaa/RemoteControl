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

    //��ȡ�����ļ���Ϣ
    WIN32_FIND_DATA wData;
    HANDLE hFile = FindFirstFile(szPath, &wData);

    char szBuff[0x10000] = { 0 };
    strcpy_s(szBuff, 0x10000, "Ŀ��·�����ļ���ϢΪ��\r\n");

    while (FindNextFile(hFile, &wData))
    {
        if (wData.cFileName[0] == '.')//����.��..
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
    //�ش���Ϣ
    HEADER head;
    head.nCmd = FILE_CHECK_INFO;
    head.nLen = strlen(szBuff) + 1 ;

    send(m_socket, (char*)&head, sizeof(head), 0);

    send(m_socket, szBuff, strlen(szBuff) + 1, 0);

}

void CFILE::upLoad(int len)
{
    //�����ļ���
    FILENAME fileName;
    recv(m_socket, (char*)&fileName, sizeof(fileName), 0);

    //�����ļ���Ϣ
    char* pBuff = new char[len];
    int nRecv = 0;
    int bytes = 0;
    while (nRecv < len)
    {
        bytes = recv(m_socket, pBuff + nRecv, len - nRecv, 0);
        nRecv += bytes;
    }
    
    //���ļ�
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
        printf("Ŀ���ļ���ʧ��\n");
        return;
    }

    //д������
    DWORD dwBytesToWrite = 0;
    if (!WriteFile(hFileDst, pBuff, len, &dwBytesToWrite, NULL))
    {
        printf("д�ļ�����\n");
        return;
    }
    if (len != dwBytesToWrite)
    {
        printf("�ļ�д�����ݶ�ʧ\n");
    }

    printf("�ļ��ɹ��ϴ�\n");
    delete[] pBuff;
    CloseHandle(hFileDst);
}

void CFILE::downLoad(int len)
{
    //��ȡ·��
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
        printf("Ŀ���ļ���ʧ��\n");
        return;
    }

    //��ȡ�ļ���С
    DWORD dwFileSize = GetFileSize(hFileDst, NULL);

    //��ȡ�ļ���
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
    char szFileName[MAX_PATH] = { 0 };//�ļ���

    int k = 0;
    int j = i + 1;
    for (; j < nLen; k++, j++)
    {
        szFileName[k] = szPath[j];
    }

    //����ͷ
    HEADER head;
    head.nCmd = FILE_DOWNLOAD_INFO;
    head.nLen = dwFileSize;
    send(m_socket, (char*)&head, sizeof(head), 0);

    //�����ļ���
    FILENAME fName;
    strcpy_s(fName.szFileName, sizeof(fName.szFileName), szFileName);
    send(m_socket, (char*)&fName, sizeof(fName), 0);

    //��ȡ�ļ�������
    while (true)
    {
        char szBuff[0x1000] = { 0 };

        //��ȡ����
        DWORD dwBytesToRead = 0;
        if (!ReadFile(hFileDst, szBuff, sizeof(szBuff), &dwBytesToRead, NULL))
        {
            printf("��ȡ�ļ�ʧ��\n");
            return;
        }

        //���û������,���˳�
        if (dwBytesToRead <= 0)
        {
            break;
        }

        //��������
        send(m_socket, szBuff, dwBytesToRead, 0);
    }
}
