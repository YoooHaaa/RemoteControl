#pragma once
#include <windows.h>
#include "protocol.h"

class CFILE
{
public:
    CFILE(SOCKET s);
    ~CFILE();

    void checkFile(char* path);
    void upLoad(int len);
    void downLoad(int len);

private:
    SOCKET m_socket;
};

