#include "Client.h"

bool Network::Client::Connect(const Network::Endpoint &endpoint) {
    Socket socket = Socket();
    if (!socket.Create()) {
        return false;
    }
    if (!socket.Connect(endpoint)) {
        OnConnectFail();
        return false;
    }
    if (!socket.SetBlocking(false)) {
        return false;
    }
    Connection.Socket = socket;
    Connection.Endpoint = endpoint;
    m_MasterFD.fd = Connection.Socket.GetHandle();
    m_MasterFD.events = POLLRDNORM;
    m_MasterFD.revents = 0;
    m_ConnectionFailed = false;
    OnConnect();
    return true;
}

void Network::Client::Close() {
    Connection.Socket.Close();
    m_ConnectionFailed = true;
    Connection.OutStream.Clear();
    Connection.InStream.Clear();
}

void Network::Client::Frame() {
    if (m_ConnectionFailed) {
        return;
    }
    if (Connection.OutStream.HasPackets()) {
        m_MasterFD.events = POLLRDNORM | POLLWRNORM;
    }
    m_UseFD = m_MasterFD;
    if(Poll(&m_UseFD, 1, 1) > 0){
        if (m_UseFD.revents & POLLERR) {
            Close();
            OnDisconnect(DisconnectReason::Error);
            return;
        }
        if (m_UseFD.revents & POLLHUP) {
            Close();
            OnDisconnect(DisconnectReason::Aborted);
            return;
        }
        if (m_UseFD.revents & POLLNVAL) {
            Close();
            OnDisconnect(DisconnectReason::InvalidSocket);
            return;
        }
        if (m_UseFD.revents & POLLRDNORM) {
            int bytesReceived;
            if (Connection.InStream.CurrentTask == PacketManagerTask::ProcessPacketSize) {
                bytesReceived = recv(m_UseFD.fd, (char*) &Connection.InStream.CurrentPacketSize + Connection.InStream.CurrentPacketBufferOffset, sizeof(uint16_t) - Connection.InStream.CurrentPacketBufferOffset, 0);
            } else {
                bytesReceived = recv(m_UseFD.fd, (char*) &Connection.Buffer + Connection.InStream.CurrentPacketBufferOffset, Connection.InStream.CurrentPacketSize - Connection.InStream.CurrentPacketBufferOffset, 0);
            }
            if (bytesReceived == 0) {
                Close();
                OnDisconnect(DisconnectReason::Error);
                return;
            }
            if (bytesReceived > 0) {
                Connection.InStream.CurrentPacketBufferOffset += bytesReceived;
                if (Connection.InStream.CurrentTask == PacketManagerTask::ProcessPacketSize) {
                    if (Connection.InStream.CurrentPacketBufferOffset == sizeof(uint16_t)) {
                        Connection.InStream.CurrentPacketSize = ntohs(Connection.InStream.CurrentPacketSize);
                        if (Connection.InStream.CurrentPacketSize > MAX_PACKET_SIZE) {
                            Close();
                            OnDisconnect(DisconnectReason::InvalidPacketLength);
                            return;
                        }
                        Connection.InStream.CurrentPacketBufferOffset = 0;
                        Connection.InStream.CurrentTask = PacketManagerTask::ProcessPacketData;
                    }
                } else {
                    if (Connection.InStream.CurrentPacketBufferOffset == Connection.InStream.CurrentPacketSize) {
                        Packet packet;
                        packet.Buffer.resize(Connection.InStream.CurrentPacketSize);
                        memcpy(&packet.Buffer[0], Connection.Buffer, Connection.InStream.CurrentPacketSize);
                        Connection.InStream.Append(packet);
                        Connection.InStream.CurrentPacketSize = 0;
                        Connection.InStream.CurrentPacketBufferOffset = 0;
                        Connection.InStream.CurrentTask = PacketManagerTask::ProcessPacketSize;
                        OnPacketReceive(packet);
                        Connection.InStream.Pop();
                    }
                }
            }
        }
        if (m_UseFD.revents & POLLWRNORM) {
            PacketManager& pm = Connection.OutStream;
            while (pm.HasPackets()) {
                if (pm.CurrentTask == PacketManagerTask::ProcessPacketSize) {
                    pm.CurrentPacketSize = pm.Retrieve().Buffer.size();
                    uint16_t bigEndianPacketSize = htons(pm.CurrentPacketSize);
                    int bytesSent = send(m_UseFD.fd, (char*) (&bigEndianPacketSize) + pm.CurrentPacketBufferOffset, sizeof(uint16_t) - pm.CurrentPacketBufferOffset, 0);
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
                    char* bufferPtr = &pm.Retrieve().Buffer[0];
                    int bytesSent = send(m_UseFD.fd, (char*) (bufferPtr) + pm.CurrentPacketBufferOffset, pm.CurrentPacketSize - pm.CurrentPacketBufferOffset, 0);
                    if (bytesSent > 0) {
                        pm.CurrentPacketBufferOffset += bytesSent;
                    }
                    if (pm.CurrentPacketBufferOffset == pm.CurrentPacketSize) {
                        pm.CurrentPacketBufferOffset = 0;
                        pm.CurrentTask = PacketManagerTask::ProcessPacketSize;
                        Packet packet = pm.Retrieve();
                        OnPacketSend(packet);
                        pm.Pop();
                    } else {
                        break;
                    }
                }
            }
            if (!Connection.OutStream.HasPackets()) {
                m_MasterFD.events = POLLRDNORM;
            }
        }
    }
}

