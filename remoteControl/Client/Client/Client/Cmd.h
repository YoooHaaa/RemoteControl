#pragma once
#include "afxwin.h"
#include "ClientDlg.h"
class CClientDlg;
// CCmd 对话框


class CCmd : public CDialogEx
{
	DECLARE_DYNAMIC(CCmd)

public:
	CCmd(CClientDlg* p, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCmd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_CMD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:




    CEdit m_CmdResult;
    afx_msg void OnBnClickedCmdSend();
    CClientDlg* pThis = nullptr;

};
