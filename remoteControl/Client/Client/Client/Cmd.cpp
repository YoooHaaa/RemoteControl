// Cmd.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "Cmd.h"
#include "afxdialogex.h"
#include "protocol.h"

// CCmd 对话框

IMPLEMENT_DYNAMIC(CCmd, CDialogEx)

CCmd::CCmd(CClientDlg* p, CWnd* pParent /*=NULL*/)
	: CDialogEx(DLG_CMD, pParent)
{
    pThis = (CClientDlg*)p;
}

CCmd::~CCmd()
{
}

void CCmd::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, EDT_CMD_RESULT, m_CmdResult);
}


BEGIN_MESSAGE_MAP(CCmd, CDialogEx)
    ON_BN_CLICKED(BTN_CMD_SEND, &CCmd::OnBnClickedCmdSend)
END_MESSAGE_MAP()


// CCmd 消息处理程序


void CCmd::OnBnClickedCmdSend()//点击发送
{
    CString cs;
    GetDlgItemText(EDT_CMD_ORDER, cs);//获取文本

    HEADER head;
    head.nCmd = CMD_ORDER;
    head.nLen = cs.GetLength();

    //发送头
    send(pThis->m_socket, (char*)&head, sizeof(head), 0);

    //发送信息
    send(pThis->m_socket, cs.GetBuffer(), cs.GetLength(), 0);

    //清空信息
    SetDlgItemText(EDT_CMD_ORDER, NULL);

}
