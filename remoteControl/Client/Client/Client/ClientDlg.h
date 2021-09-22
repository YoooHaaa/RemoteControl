
// ClientDlg.h : 头文件
//

#pragma once

#include "Cmd.h"
#include "FILE.h"
#include "SCREEN.h"
#include "protocol.h"
#include <ctime>

class CCmd;
class CFILE;
class CSCREEN;

// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()



public:
    int m_socket;

    //三个功能对话框
    CCmd* pCmd = nullptr;
    CFILE* pFile = nullptr;
    CSCREEN* pScreen = nullptr;

    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton3();
    int create_thread();
    int GetCmdMsg();
    int GetScreenMsg(int len);
    void showScreen(int width, int high, char* buff);
    PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);

    int GetFileMsg(int type, int len);

    bool m_bThread = false;

private:
    
public:
    afx_msg void OnBnClickedButton4();
};
