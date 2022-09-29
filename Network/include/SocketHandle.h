#pragma once
#include "Core.h"
namespace Network {
#ifdef NETWORK_WIN
    typedef SOCKET SocketHandle;
#else
    typedef int SocketHandle;
#endif

}