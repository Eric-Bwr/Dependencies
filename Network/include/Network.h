#pragma once
#include "Core.h"

namespace Network{
#ifdef NETWORK_WIN
    bool Initialize(){
        WSADATA wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            return false;
        }
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            return false;
        }
        return true;
    }
    void Terminate(){
        WSACleanup();
    }
#endif
}