#include "PacketStream.h"

namespace Network {
    void PacketOutStream::Append(std::shared_ptr<Packet>& packet) {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Packets.emplace(packet);
    }
    std::shared_ptr<Packet>& PacketOutStream::Front() {
        std::unique_lock<std::mutex> lock(m_Mutex);
        return m_Packets.front();
    }
    void PacketOutStream::Pop() {
        std::unique_lock<std::mutex> lock(m_Mutex);
        m_Packets.pop();
    }
}