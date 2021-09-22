// SCREEN.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "SCREEN.h"
#include "afxdialogex.h"


// CSCREEN 对话框

IMPLEMENT_DYNAMIC(CSCREEN, CDialogEx)



CSCREEN::CSCREEN(CClientDlg* p, CWnd* pParent /*=NULL*/)
	: CDialogEx(DLG_SCREEN, pParent)
{
    pThis = (CClientDlg*)p;


    
}

CSCREEN::~CSCREEN()
{
}

void CSCREEN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSCREEN, CDialogEx)
    ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CSCREEN 消息处理程序


BOOL CSCREEN::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化




    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CSCREEN::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 在此处添加消息处理程序代码
                       // 不为绘图消息调用 CDialogEx::OnPaint()


}


void CSCREEN::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (nSBCode != SB_ENDSCROLL)
    {
        SCROLLINFO hStructure;
        GetScrollInfo(SB_HORZ, &hStructure);
        hStructure.nPos = nPos;
        SetScrollInfo(SB_HORZ, &hStructure);
    }

    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSCREEN::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    if (nSBCode != SB_ENDSCROLL)
    {
        SCROLLINFO hStructure;
        GetScrollInfo(SB_VERT, &hStructure);
        hStructure.nPos = nPos;
        SetScrollInfo(SB_VERT, &hStructure);
    }

    CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CSCREEN::PreTranslateMessage(MSG* pMsg)
{
    if (m_bCtrl)
    {
        switch (pMsg->message)
        {
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_MOUSEMOVE:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            MSG msg;
            memcpy(&msg, pMsg, sizeof(MSG));

            HEADER head;
            head.nCmd = SCREEN_COMMAND;
            head.nLen = sizeof(MSG);

            send(pThis->m_socket, (char*)&head, sizeof(head), 0);

            send(pThis->m_socket, (char*)&msg, sizeof(msg), 0);
        }
        default: break;
        }
    }
    
    return CDialogEx::PreTranslateMessage(pMsg);
}
