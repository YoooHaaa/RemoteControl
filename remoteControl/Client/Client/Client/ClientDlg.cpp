
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"


#include <WinSock2.h>
#include <tchar.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CClientDlg �Ի���



CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    //����
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1152);
    /************************************************************************************/
    if (inet_pton(AF_INET, "192.168.72.131", &addr.sin_addr) < 0) 
    {
        AfxMessageBox(TEXT("inet_pton ERROR!"));
    }
    
    if (connect(m_socket, (sockaddr*)&addr, sizeof(addr)) < 0) 
    {
        int n = WSAGetLastError();
        AfxMessageBox(TEXT("connect ERROR!"));
    }
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON2, &CClientDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON1, &CClientDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON3, &CClientDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CClientDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnBnClickedButton2()//Զ��CMD
{
    //�����߳̽�����Ϣ
    if (!m_bThread)
    {
        create_thread();
        m_bThread = true;
    }

    //�����Ի���
    pCmd = new CCmd(this);
    pCmd->Create(DLG_CMD);
    pCmd->ShowWindow(SW_SHOW);

    
}


void CClientDlg::OnBnClickedButton1()//Զ����Ļ
{
    //�����߳̽�����Ϣ
    if (!m_bThread)
    {
        create_thread();
        m_bThread = true;
    }

    //�����Ի���
    pScreen = new CSCREEN(this);
    pScreen->Create(DLG_SCREEN);
    pScreen->ShowWindow(SW_SHOW);

    pScreen->m_bCtrl = true;

    HEADER head;
    head.nCmd = SCREEN_CTRL;
    send(m_socket, (char*)&head, sizeof(head), 0);
}

void CClientDlg::OnBnClickedButton4()//ֹͣԶ��
{
    HEADER head;
    head.nCmd = SCREEN_OVER;
    send(m_socket, (char*)&head, sizeof(head), 0);

    pScreen->m_bCtrl = false;
}

void CClientDlg::OnBnClickedButton3()//Զ���ļ�
{
    //�����߳̽�����Ϣ
    if (!m_bThread)
    {
        create_thread();
        m_bThread = true;
    }

    //�����Ի���
    pFile = new CFILE(this);
    pFile->Create(DLG_FILE);
    pFile->ShowWindow(SW_SHOW);


}

DWORD WINAPI GetMsgThread(LPVOID lpParameter)
{
    CClientDlg* pthis = (CClientDlg*)lpParameter;

    while (true)
    {
        HEADER head;
        recv(pthis->m_socket, (char*)&head, sizeof(head), 0);

        switch (head.nCmd)
        {
        case CMD_RESULT:
        {
            pthis->GetCmdMsg();
            break;
        }
        case SCREEN_INFO:
        {
            pthis->GetScreenMsg(head.nLen);
            break;
        }
        case FILE_CHECK_INFO:
        {
            pthis->GetFileMsg(FILE_CHECK_INFO, head.nLen);
            break;
        }
        case FILE_DOWNLOAD_INFO:
        {
            pthis->GetFileMsg(FILE_DOWNLOAD_INFO, head.nLen);
            break;
        }
        default:break;
        }


    }
}

int CClientDlg::create_thread()
{
    DWORD dwThreadId = 0;
    HANDLE hThread = CreateThread(
        NULL,
        0,
        (LPTHREAD_START_ROUTINE)GetMsgThread,
        (LPVOID)this,
        0, //��������
        &dwThreadId);
    if (hThread == NULL)
    {
        AfxMessageBox(TEXT("create Thread fail!"));
    }

    return 0;
}

int CClientDlg::GetCmdMsg()
{
    clock_t tBegin = clock();
    while (true)
    {
        clock_t tEnd = clock();
        if (tEnd - tBegin > 10000)
        {
            return 0;
        }

        char szBuff[0x1000] = { 0 };
        recv(m_socket, szBuff, 0x1000, 0);

        //д����Ϣ
        CString cs;
        pCmd->GetDlgItemText(EDT_CMD_RESULT, cs);
        cs += szBuff;
        pCmd->SetDlgItemText(EDT_CMD_RESULT, cs);

    }

    return 0;
}

int CClientDlg::GetScreenMsg(int len)
{
    SIZESCREEN sSize;
    recv(m_socket, (char*)&sSize, sizeof(sSize), 0);

    
    void* pBuff = new char[len];
    int nRecv = 0;
    int bytes = 0;
    while (nRecv < len)
    {
        bytes = recv(m_socket, (char*)pBuff + nRecv, len - nRecv, 0);
        nRecv += bytes;
    }

    //��ѹ
    using UNCOMPRESS = int(*)(void*, unsigned long*, void*, unsigned long);
    HMODULE hDll = LoadLibrary(".\\zlib1.dll");
    UNCOMPRESS uncompress = (UNCOMPRESS)GetProcAddress(hDll, "uncompress");

    LPVOID pInfo = (LPBYTE)GlobalAlloc(GMEM_FIXED, sSize.nSize);
    unsigned long TotalSize;
    uncompress(pInfo, &TotalSize, pBuff, len);

    /*****************************************/
    /*CString cs;
    cs.Format("���ճ���%d", len);
    AfxMessageBox(cs);*/
    /*****************************************/

    showScreen(sSize.nWidth, sSize.nHigh, (char*)pInfo);

    GlobalFree((HGLOBAL)pInfo);
    delete[] pBuff;
    return 0;
}

void CClientDlg::showScreen(int width, int high, char * buff)
{
    HDC hdc = ::GetDC(pScreen->m_hWnd);

    //�����ڴ�dc
    HDC hdcCompatible = ::CreateCompatibleDC(hdc);
    //check

    HBITMAP hbmScreen = CreateCompatibleBitmap(hdc, width, high);

    //ѡ��λͼ
    SelectObject(hdcCompatible, hbmScreen);

    SetBitmapBits(hbmScreen, width * high * 4, buff);
    //LocalFree(pbi);

    //����λͼ
    BitBlt(hdc,
        0, 0,
        width, high,
        hdcCompatible,
        0, 0,
        SRCCOPY);


    DeleteDC(hdcCompatible);
    DeleteObject(hbmScreen);
    DeleteDC(hdc);
}


/*����ĺ����Ǹ������Ҳ��֪��*/
#define errhandler

PBITMAPINFO CClientDlg::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
        errhandler("GetObject", hwnd);

    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

    if (cClrBits != 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER) +
            sizeof(RGBQUAD) * (1 << cClrBits));
    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER));

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    pbmi->bmiHeader.biCompression = BI_RGB;

    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
        * pbmi->bmiHeader.biHeight;

    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}

int CClientDlg::GetFileMsg(int type, int len)
{
    switch (type)
    {
    case FILE_CHECK_INFO:
    {
        char* pszFile = new char[len];
        recv(m_socket, pszFile, len, 0);

        CString cs;
        cs += pszFile;
        pFile->SetDlgItemText(EDT_SHOW, cs);

        delete[] pszFile;
        break;
    }
    case FILE_DOWNLOAD_INFO:
    {
        //�����ļ���
        FILENAME fileName;
        recv(m_socket, (char*)&fileName, sizeof(fileName), 0);

        //�����ļ���Ϣ
        char* pBuff = new char[len];
        int nRecv = 0;
        int bytes = 0;
        while (nRecv < len)
        {
            bytes = recv(m_socket, pBuff + nRecv, len - nRecv, 0);
            nRecv += bytes;
        }

        //���ļ�
        char szFilePath[MAX_PATH] = { 0 };
        strcpy_s(szFilePath, MAX_PATH, ".\\");
        strcat_s(szFilePath, MAX_PATH, fileName.szFileName);

        HANDLE hFileDst = CreateFile(
            szFilePath,
            GENERIC_WRITE,
            FILE_SHARE_READ,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);
        if (hFileDst == INVALID_HANDLE_VALUE)
        {
            AfxMessageBox(TEXT("Ŀ���ļ���ʧ��\n"));
            return 0;
        }

        //д������
        DWORD dwBytesToWrite = 0;
        if (!WriteFile(hFileDst, pBuff, len, &dwBytesToWrite, NULL))
        {
            AfxMessageBox(TEXT("д�ļ�����\n"));
            return 0;
        }
        if (len != dwBytesToWrite)
        {
            AfxMessageBox(TEXT("�ļ�д�����ݶ�ʧ\n"));
        }

        AfxMessageBox(TEXT("�ļ��ɹ�����\n"));

        delete[] pBuff;
        CloseHandle(hFileDst);
        break;
    }
    default:break;
    }


    return 0;
}

