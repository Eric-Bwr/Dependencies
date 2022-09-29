#include "Server.h"
#include <thread>

bool Network::Server::Initialize(const Network::Endpoint &endpoint) {
    m_MasterFD.clear();
    m_Connections.clear();
    m_Server = Socket();
    if (!m_Server.Create()) {
        return false;
    }
    if (!m_Server.SetBlocking(false)) {
        return false;
    }
    if (!m_Server.Listen(endpoint)) {
        return false;
    }
    PollFD serverFD = {};
    serverFD.fd = m_Server.GetHandle();
    serverFD.events = POLLRDNORM;
    serverFD.revents = 0;
    m_MasterFD.push_back(serverFD);
    m_Init = true;
    return true;
}

void Network::Server::Terminate() {
    m_MasterFD.clear();
    m_UseFD.clear();
    m_Connections.clear();
    m_Server.Close();
}

void Network::Server::Frame() {
    if(!m_Init){
        return;
    }
    for (int i = 0; i < m_Connections.size(); i++) {
        if (m_Connections[i].OutStream.HasPackets()) {
            m_MasterFD[i + 1].events = POLLRDNORM | POLLWRNORM;
        }
    }
    m_UseFD = m_MasterFD;
    if(Poll(m_UseFD.data(), m_UseFD.size(), 1) > 0){

        if (m_UseFD[0].revents & POLLRDNORM) {
            Socket clientSocket;
            Endpoint clientEndpoint;
            if (!m_Server.Accept(clientSocket, clientEndpoint)) {
                return;
            }
            TCPConnection connection;
            connection.Socket = clientSocket;
            connection.Endpoint = clientEndpoint;
            connection.Index = clientSocket.GetHandle();
            OnConnect(connection);
            m_Connections.emplace_back(connection);
            PollFD clientFD = {};
            clientFD.fd = clientSocket.GetHandle();
            clientFD.events = POLLRDNORM;
            clientFD.revents = 0;
            m_MasterFD.push_back(clientFD);
        }

        for (int i = m_UseFD.size() - 1; i >= 1; i--) {
            int index = i - 1;
            TCPConnection &connection = m_Connections[index];
            if (m_UseFD[i].revents & POLLERR) {
                CloseConnection(index);
                OnDisconnect(connection, DisconnectReason::Error);
                continue;
            }
            if (m_UseFD[i].revents & POLLHUP) {
                CloseConnection(index);
                OnDisconnect(connection, DisconnectReason::Aborted);
                continue;
            }
            if (m_UseFD[i].revents & POLLNVAL) {
                CloseConnection(index);
                OnDisconnect(connection, DisconnectReason::InvalidSocket);
                continue;
            }
            if (m_UseFD[i].revents & POLLRDNORM) {
                int bytesReceived;
                if (connection.InStream.CurrentTask == PacketManagerTask::ProcessPacketSize) {
                    bytesReceived = recv(m_UseFD[i].fd, (char*) &connection.InStream.CurrentPacketSize + connection.InStream.CurrentPacketBufferOffset, sizeof(uint16_t) - connection.InStream.CurrentPacketBufferOffset, 0);
                } else {
                    bytesReceived = recv(m_UseFD[i].fd, (char*) &connection.Buffer + connection.InStream.CurrentPacketBufferOffset, connection.InStream.CurrentPacketSize - connection.InStream.CurrentPacketBufferOffset, 0);
                }
                if (bytesReceived == 0) {
                    CloseConnection(index);
                    OnDisconnect(connection, DisconnectReason::Error);
                    continue;
                }
                if (bytesReceived == SOCKET_ERROR) {
                    CloseConnection(index);
                    OnDisconnect(connection, DisconnectReason::Error);
                    continue;
                }
                if (bytesReceived > 0) {
                    connection.InStream.CurrentPacketBufferOffset += bytesReceived;
                    if (connection.InStream.CurrentTask == PacketManagerTask::ProcessPacketSize) {
                        if (connection.InStream.CurrentPacketBufferOffset == sizeof(uint16_t)) {
                            connection.InStream.CurrentPacketSize = ntohs(connection.InStream.CurrentPacketSize);
                            if (connection.InStream.CurrentPacketSize > MAX_PACKET_SIZE) {
                                CloseConnection(index);
                                OnDisconnect(connection,DisconnectReason::InvalidPacketLength);
                                continue;
                            }
                            connection.InStream.CurrentPacketBufferOffset = 0;
                            connection.InStream.CurrentTask = PacketManagerTask::ProcessPacketData;
                        }
                    } else {
                        if (connection.InStream.CurrentPacketBufferOffset == connection.InStream.CurrentPacketSize) {
                            Packet packet;
                            packet.Buffer.resize(connection.InStream.CurrentPacketSize);
                            memcpy(&packet.Buffer[0], connection.Buffer, connection.InStream.CurrentPacketSize);
                            connection.InStream.CurrentPacketSize = 0;
                            connection.InStream.CurrentPacketBufferOffset = 0;
                            connection.InStream.CurrentTask = PacketManagerTask::ProcessPacketSize;
                            OnPacketReceive(connection, packet);
                        }
                    }
                }
            }
            if (m_UseFD[i].revents & POLLWRNORM) {
                PacketManager& pm = connection.OutStream;
                while (pm.HasPackets()) {
                    if (pm.CurrentTask == PacketManagerTask::ProcessPacketSize) {
                        pm.CurrentPacketSize = pm.Retrieve().Buffer.size();
                        uint16_t bigEndianPacketSize = htons(pm.CurrentPacketSize);
                        int bytesSent = send(m_UseFD[i].fd, (char*) (&bigEndianPacketSize) + pm.CurrentPacketBufferOffset, sizeof(uint16_t) - pm.CurrentPacketBufferOffset, 0);
                        if (bytesSent > 0) {
                            pm.CurrentPacketBufferOffset += bytesSent;
                        }
                        if (pm.CurrentPacketBufferOffset == sizeof(uint16_t)) {
                            pm.CurrentPacketBufferOffset = 0;
                            pm.CurrentTask = PacketManagerTask::ProcessPacketData;
                        } else {
                            break;
                        }
                    } else {
                        char* m_BufferPtr = &pm.Retrieve().Buffer[0];
                        int bytesSent = send(m_UseFD[i].fd, (char*) (m_BufferPtr) + pm.CurrentPacketBufferOffset, pm.CurrentPacketSize - pm.CurrentPacketBufferOffset, 0);
                        if (bytesSent > 0) {
                            pm.CurrentPacketBufferOffset += bytesSent;
                        }
                        if (pm.CurrentPacketBufferOffset == pm.CurrentPacketSize) {
                            pm.CurrentPacketBufferOffset = 0;
                            pm.CurrentTask = PacketManagerTask::ProcessPacketSize;
                            Packet packet = pm.Retrieve();
                            OnPacketSend(connection, packet);
                            pm.Pop();
                        } else {
                            break;
                        }
                    }
                }
                if (!pm.HasPackets()) {
                    m_MasterFD[i].events = POLLRDNORM;
                }
            }
        }

    }
}

void Network::Server::CloseConnection(int index) {
    TCPConnection &connection = m_Connections.at(index);
    m_MasterFD.erase(m_MasterFD.begin() + (index + 1));
    m_UseFD.erase(m_UseFD.begin() + (index + 1));
    connection.Socket.Close();
    m_Connections.erase(m_Connections.begin() + index);
}

uint64_t Network::Server::GetConnectionSize() const{
    return m_MasterFD.size() - 1;
}

