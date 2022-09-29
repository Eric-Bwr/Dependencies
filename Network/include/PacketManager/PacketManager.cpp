#include "PacketManager.h"

bool Network::PacketManager::HasPackets() const {
    return !m_Packets.empty();
}

Network::Packet Network::PacketManager::Retrieve() const {
    return m_Packets.front();
}

bool Network::PacketManager::Append(const Network::Packet &packet) {
    if(m_Packets.size() < 128){
        m_Packets.emplace(packet);
        return true;
    }
    return false;
}

void Network::PacketManager::Clear() {
    while(HasPackets()){
        Retrieve();
        Pop();
    }
}

void Network::PacketManager::Pop() {
    m_Packets.pop();
}
