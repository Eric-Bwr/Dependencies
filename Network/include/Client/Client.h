#pragma once

#include <TCPConnection.h>
#include "Endpoint/Endpoint.h"
#include "Core.h"
#include "../DisconnectReason.h"
#include <thread>

namespace Network{
    class Client {
    public:
        Client() = default;
        bool Connect(const Endpoint& endpoint);
        void Close();
        void Frame();
        virtual void OnConnect(){};
        virtual void OnConnectFail(){};
        virtual void OnDisconnect(DisconnectReason reason){};
        virtual void OnPacketReceive(Packet& packet){};
        virtual void OnPacketSend(Packet& packet){};
        TCPConnection Connection;
    private:
        bool m_ConnectionFailed = false;
        PollFD m_MasterFD;
        PollFD m_UseFD;
    };

}