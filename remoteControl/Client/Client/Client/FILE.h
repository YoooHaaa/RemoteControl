#pragma once
#include "ClientDlg.h"
#include "afxcmn.h"





// CFILE 对话框
class CClientDlg;
class CFILE : public CDialogEx
{
	DECLARE_DYNAMIC(CFILE)

public:
	CFILE(CClientDlg* p, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFILE();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()



public:

    CClientDlg* pThis = nullptr;



    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
};
