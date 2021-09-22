// Cmd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Cmd.h"
#include "afxdialogex.h"
#include "protocol.h"

// CCmd �Ի���

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


// CCmd ��Ϣ�������


void CCmd::OnBnClickedCmdSend()//�������
{
    CString cs;
    GetDlgItemText(EDT_CMD_ORDER, cs);//��ȡ�ı�

    HEADER head;
    head.nCmd = CMD_ORDER;
    head.nLen = cs.GetLength();

    //����ͷ
    send(pThis->m_socket, (char*)&head, sizeof(head), 0);

    //������Ϣ
    send(pThis->m_socket, cs.GetBuffer(), cs.GetLength(), 0);

    //�����Ϣ
    SetDlgItemText(EDT_CMD_ORDER, NULL);

}
