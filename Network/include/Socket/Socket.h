#pragma once
#include "Core.h"
#include "SocketHandle.h"
#include "SocketOption.h"
#include "Endpoint/Endpoint.h"

namespace Network {
    class Socket {
    public:
        Socket(SocketHandle handle = INVALID_SOCKET);
        bool Create();
        bool Connect(const Endpoint& endpoint) const;
        bool Close();
        bool Bind(const Endpoint& endpoint) const;
        bool Listen(const Endpoint& endpoint) const;
        bool Accept(Socket& socket, Endpoint& endpoint) const;
        bool SetSocketOption(SocketOption option, int value) const;
        bool SetBlocking(bool blocking) const;
        bool Send(void* data, int size, int& bytesSent) const;
        bool Receive(void* destination, int size, int& bytesReceived) const;
        bool SendAll(void* data, int size) const;
        bool ReceiveAll(void* destination, int size) const;
        SocketHandle  GetHandle() const {return m_Handle;}
    private:
        SocketHandle m_Handle = INVALID_SOCKET;
    };
}