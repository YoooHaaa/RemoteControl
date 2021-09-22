#pragma once
#include "ClientDlg.h"




// CSCREEN 对话框
class CClientDlg;
class CSCREEN : public CDialogEx
{
	DECLARE_DYNAMIC(CSCREEN)

public:
	CSCREEN(CClientDlg* p, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSCREEN();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_SCREEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
    CClientDlg* pThis = nullptr;
    bool m_bCtrl = true;

    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();


    HDC m_dc;
    afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};
