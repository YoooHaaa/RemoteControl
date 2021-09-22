#pragma once
#include <windows.h>
#include "protocol.h"

class CSCREEN
{
public:
    CSCREEN(SOCKET s);
    ~CSCREEN();

    PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);
    void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC);

    int screenSnap();



private:
    SOCKET m_socket;

};

