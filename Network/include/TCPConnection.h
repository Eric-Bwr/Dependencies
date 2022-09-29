#pragma once
#include "Socket/Socket.h"
#include "PacketManager/PacketManager.h"
#include <queue>

namespace Network{
    struct TCPConnection{
        Socket Socket;
        Endpoint Endpoint;
        uint64_t Index;
        PacketManager OutStream;
        PacketManager InStream;
        char Buffer[MAX_PACKET_SIZE];
    };
}