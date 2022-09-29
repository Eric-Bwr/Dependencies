#include <iostream>
#include "Socket.h"

Network::Socket::Socket(Network::SocketHandle handle) {
    m_Handle = handle;
}

bool Network::Socket::Create() {
    if (m_Handle != INVALID_SOCKET) {
        return false;
    }
    m_Handle = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_Handle == INVALID_SOCKET) {
        return false;
    }
    if (!SetSocketOption(SocketOption::TCPNoDelay, true)) {
        return false;
    }
    return true;
}

bool Network::Socket::Connect(const Network::Endpoint &endpoint) const {
    int result;
    sockaddr_in address = endpoint.GetSockAddressIPv4();
    result = connect(m_Handle, (sockaddr * ) & address, sizeof(sockaddr_in));

    return result == 0;
}

bool Network::Socket::Close() {
    if (m_Handle == INVALID_SOCKET) {
        return false;
    }
    int result = CloseSocket(m_Handle);
    if (result != 0) {
        return false;
    }
    m_Handle = INVALID_SOCKET;
    return true;
}

bool Network::Socket::Bind(const Network::Endpoint &endpoint) const {
    sockaddr_in address = endpoint.GetSockAddressIPv4();
    int result = bind(m_Handle, (sockaddr * ) & address, sizeof(sockaddr_in));
    return result == 0;
}

bool Network::Socket::Listen(const Network::Endpoint &endpoint) const {
    if (!Bind(endpoint)) {
        return false;
    }
    int result = listen(m_Handle, SOMAXCONN);
    if (result != 0) {
        return false;
    }
    return true;
}

bool Network::Socket::Accept(Network::Socket &socket, Network::Endpoint &endpoint) const {
    sockaddr_in address = {};
    socklen len = sizeof(sockaddr_in);
    SocketHandle clientHandle = accept(m_Handle, (sockaddr * ) & address, &len);
    if (clientHandle == INVALID_SOCKET) {
        return false;
    }
    endpoint = Endpoint((sockaddr * ) & address);
    socket = Socket(clientHandle);
    return true;
}

bool Network::Socket::SetSocketOption(Network::SocketOption option, int value) const {
    int result;
    switch (option) {
        case SocketOption::TCPNoDelay:
            result = setsockopt(m_Handle, IPPROTO_TCP, TCP_NODELAY, (const char *) &value, sizeof(value));
            break;
        default:
            return false;
    }
    return !result;
}

bool Network::Socket::SetBlocking(bool blocking) const {
#ifdef _WIN32
    unsigned long mode = blocking ? 0 : 1;
    return (ioctlsocket(m_Handle, FIONBIO, &mode) == 0);
#else
    int flags = fcntl(m_Handle, F_GETFL, 0);
    if (flags == -1) return false;
    flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
    return (fcntl(m_Handle, F_SETFL, flags) == 0);
#endif
}

bool Network::Socket::Send(void *data, int size, int &bytesSent) const {
    bytesSent = send(m_Handle, (const char *) data, size, 0);
    if (bytesSent == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool Network::Socket::Receive(void *destination, int size, int &bytesReceived) const {
    bytesReceived = recv(m_Handle, (char *) destination, size, 0);
    if (bytesReceived == 0) {
        return false;
    }
    if (bytesReceived == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool Network::Socket::SendAll(void *data, int size) const {
    int totalSend = 0;
    while (totalSend < size) {
        int remaining = size - totalSend;
        int sent = 0;
        char *bufferOffset = (char *) data + totalSend;
        if (!Send(bufferOffset, remaining, sent)) {
            return false;
        }
        totalSend += sent;
    }
    return true;
}

bool Network::Socket::ReceiveAll(void *destination, int size) const {
    int totalReceive = 0;
    while (totalReceive < size) {
        int remaining = size - totalReceive;
        int received = 0;
        char *bufferOffset = (char *) destination + totalReceive;
        if (!Receive(bufferOffset, remaining, received)) {
            return false;
        }
        totalReceive += received;
    }
    return true;
}


