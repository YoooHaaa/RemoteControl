#pragma once
#include "ClientDlg.h"
#include "afxcmn.h"





// CFILE �Ի���
class CClientDlg;
class CFILE : public CDialogEx
{
	DECLARE_DYNAMIC(CFILE)

public:
	CFILE(CClientDlg* p, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFILE();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = DLG_FILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()



public:

    CClientDlg* pThis = nullptr;



    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
};
