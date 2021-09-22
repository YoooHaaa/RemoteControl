// FILE.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "FILE.h"
#include "afxdialogex.h"


// CFILE 对话框

IMPLEMENT_DYNAMIC(CFILE, CDialogEx)

CFILE::CFILE(CClientDlg* p, CWnd* pParent /*=NULL*/)
	: CDialogEx(DLG_FILE, pParent)
{
    pThis = (CClientDlg*)p;
}

CFILE::~CFILE()
{
}

void CFILE::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CFILE, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CFILE::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CFILE::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CFILE::OnBnClickedButton3)
END_MESSAGE_MAP()


// CFILE 消息处理程序


void CFILE::OnBnClickedButton1()//查看盘文件
{
    CString cs;
    GetDlgItemText(EDT_CHECK, cs);

    HEADER head;
    head.nCmd = FILE_CHECK;
    head.nLen = cs.GetLength();

    send(pThis->m_socket, (char*)&head, sizeof(head), 0);

    send(pThis->m_socket, cs.GetBuffer(), cs.GetLength(), 0);
}


void CFILE::OnBnClickedButton2()//下载文件
{
    CString cs;
    GetDlgItemText(EDT_DOWNLOAD, cs);

    HEADER head;
    head.nCmd = FILE_DOWNLOAD;
    head.nLen = cs.GetLength();

    send(pThis->m_socket, (char*)&head, sizeof(head), 0);

    send(pThis->m_socket, cs.GetBuffer(), cs.GetLength(), 0);

}


void CFILE::OnBnClickedButton3()//上传文件
{
    //获取文件路径
    CString cs;
    GetDlgItemText(EDT_UPLOAD, cs);

    //获取文件名
    int nLen = cs.GetLength();
    int i = nLen - 1;
    while (true)
    {
        if (cs.GetBuffer()[i] == '\\')
        {
            break;
        }
        i--;
    }
    char szFileName[MAX_PATH] = { 0 };

    int k = 0;
    int j = i + 1;
    for (; j < nLen; k++, j++)
    {
        szFileName[k] = cs.GetBuffer()[j];
    }

    //打开文件
    HANDLE hFileSrc = CreateFile(
        cs.GetBuffer(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (hFileSrc == INVALID_HANDLE_VALUE)
    {
        AfxMessageBox(_T("源文件打开失败"));
        return;
    }

    //获取文件大小
    DWORD dwFileSize = GetFileSize(hFileSrc, NULL);

    HEADER head;
    head.nCmd = FILE_UPLOAD;
    head.nLen = dwFileSize;

    //发送头
    send(pThis->m_socket, (char*)&head, sizeof(head), 0);

    //发送文件名
    FILENAME fName;
    strcpy_s(fName.szFileName, sizeof(fName.szFileName), szFileName);
    send(pThis->m_socket, (char*)&fName, sizeof(fName), 0);
    
    //读取文件并发送
    while(true)
    {
        char szBuff[0x1000] = { 0 };

        //读取数据
        DWORD dwBytesToRead = 0;
        if (!ReadFile(hFileSrc, szBuff, sizeof(szBuff), &dwBytesToRead, NULL))
        {
            AfxMessageBox(_T("读取失败!!!"));
        }

        //如果没有数据,则退出
        if (dwBytesToRead <= 0)
        {
            break;
        }

        //发送数据
        send(pThis->m_socket, szBuff, dwBytesToRead, 0);
    }

}
