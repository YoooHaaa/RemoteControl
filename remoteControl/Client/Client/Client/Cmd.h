#pragma once
#include "afxwin.h"
#include "ClientDlg.h"
class CClientDlg;
// CCmd �Ի���


class CCmd : public CDialogEx
{
	DECLARE_DYNAMIC(CCmd)

public:
	CCmd(CClientDlg* p, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCmd();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CMD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:




    CEdit m_CmdResult;
    afx_msg void OnBnClickedCmdSend();
    CClientDlg* pThis = nullptr;

};
