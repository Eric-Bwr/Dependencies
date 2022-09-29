#pragma once
namespace Network{
    enum class DisconnectReason{
        Error,Aborted,InvalidSocket,InvalidPacketLength
    };
}