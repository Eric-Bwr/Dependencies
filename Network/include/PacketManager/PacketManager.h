#pragma once
#include "Core.h"
#include "Packet/Packet.h"
#include <queue>

namespace Network{
    enum class PacketManagerTask{
        ProcessPacketData, ProcessPacketSize
    };

    class PacketManager {
    public:
        PacketManager() = default;
        bool HasPackets() const;
        Packet Retrieve() const;
        bool Append(const Packet& packet);
        void Clear();
        void Pop() ;
        int CurrentPacketBufferOffset = 0;
        uint16_t CurrentPacketSize = 0;
        PacketManagerTask CurrentTask = PacketManagerTask::ProcessPacketSize;
    private:
        std::queue<Packet> m_Packets;
    };

}