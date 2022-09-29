#include "Endpoint.h"
Network::Endpoint::Endpoint(sockaddr *address) {
        auto* addressV4 = reinterpret_cast<sockaddr_in*>(address);
        m_Port = ntohs(addressV4->sin_port);
        m_IPBytes.resize(sizeof(uint32_t));
        memcpy(&m_IPBytes[0], &addressV4->sin_addr, sizeof(uint32_t));
        m_IP.resize(16);
        inet_ntop(AF_INET, &addressV4->sin_addr, &m_IP[0], 16);
        m_Hostname = m_IP;
}

Network::Endpoint::Endpoint(const char *ip, unsigned short port) {
    m_Port = port;
    in_addr address;
    if(inet_pton(AF_INET,ip,&address)){
        if(address.s_addr != INADDR_NONE){
            m_IP = ip;
            m_Hostname = ip;
            m_IPBytes.resize(sizeof(uint32_t));
            memcpy(&m_IPBytes[0],&address.s_addr,sizeof(uint32_t));
            return;
        }
    }
    addrinfo hints;
    hints.ai_family = AF_INET;
    addrinfo* hostInfo = nullptr;
    if(getaddrinfo(ip,0,&hints,&hostInfo) == 0){
        sockaddr_in* hostAddress = reinterpret_cast<sockaddr_in*>(hostInfo->ai_addr);
        m_IP.resize(16);
        inet_ntop(AF_INET, &hostAddress->sin_addr,&m_IP[0],16);
        m_Hostname = ip;
        uint32_t ipLong = hostAddress->sin_addr.s_addr;
        m_IPBytes.resize(sizeof(uint32_t));
        memcpy(&m_IPBytes[0],&ipLong, sizeof(uint32_t));
        freeaddrinfo(hostInfo);
        return;
    }
}

sockaddr_in Network::Endpoint::GetSockAddressIPv4() const {
    sockaddr_in address;
    address.sin_family = AF_INET;
    memcpy(&address.sin_addr, &m_IPBytes[0], sizeof(uint32_t));
    address.sin_port = htons(m_Port);
    return address;
}
