#pragma once

enum MYCMD
{
    //SCREEN
    //Clent
    SCREEN_CTRL,
    SCREEN_OVER,
    SCREEN_COMMAND,
    //Server
    SCREEN_INFO,


    //CMD
    //Client
    CMD_ORDER,
    //Server
    CMD_RESULT,

    //FILE
    //Client
    FILE_CHECK,
    FILE_DOWNLOAD,
    FILE_UPLOAD,
    //Server
    FILE_CHECK_INFO,
    FILE_DOWNLOAD_INFO
};

#pragma pack(1)
struct HEADER
{
    HEADER()
    {
        nCmd = -1;
        nLen = 0;
    }
    int nCmd;        //命令
    int nLen;        //信息总长度
};

struct SIZESCREEN//屏幕尺寸
{
    int nWidth;
    int nHigh;
    int nSize;
};

struct FILENAME
{
    char szFileName[64] = { 0 };
};
