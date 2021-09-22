#pragma once
#include "ClientDlg.h"




// CSCREEN �Ի���
class CClientDlg;
class CSCREEN : public CDialogEx
{
	DECLARE_DYNAMIC(CSCREEN)

public:
	CSCREEN(CClientDlg* p, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSCREEN();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_SCREEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
