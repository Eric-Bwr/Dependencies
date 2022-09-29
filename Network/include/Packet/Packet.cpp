#include "Packet.h"

Network::Packet::Packet(uint16_t type) {
    Clear();
    SetPacketType(type);
}

void Network::Packet::Clear() {
    Buffer.resize(sizeof(uint16_t));
    BufferOffset = sizeof(uint16_t);
}

void Network::Packet::SetPacketType(uint16_t packetType) {
    auto* packetTypePtr = reinterpret_cast<uint16_t*>(&Buffer[0]);
    *packetTypePtr = packetType;
}

uint16_t Network::Packet::GetPacketType() const{
    const auto* packetTypePtr = reinterpret_cast<const uint16_t*>(&Buffer[0]);
    return *packetTypePtr;
}
bool Network::Packet::IsEmpty() {
    return Buffer.size() == sizeof(uint16_t);
}

void Network::Packet::Append(const void *data, uint32_t size) {
    if (Buffer.size() + size > MAX_PACKET_SIZE) {
        return;
    }
    Buffer.insert(Buffer.end(), (char*) data, (char*) data + size);
}

void Network::Packet::AppendUint8(uint8_t data) {
    Append(&data, sizeof(uint8_t));
}

void Network::Packet::AppendUint16(uint16_t data) {
    Append(&data, sizeof(uint16_t));
}

void Network::Packet::AppendUint32(uint32_t data) {
    Append(&data, sizeof(uint32_t));
}

void Network::Packet::AppendUint64(uint64_t data) {
    Append(&data, sizeof(uint64_t));
}

void Network::Packet::AppendString(const std::string &data) {
    AppendUint32(data.size());
    Append(data.data(), data.size());
}

void Network::Packet::AppendBool(bool data) {
    Append(&data, sizeof(bool));
}
void Network::Packet::AppendFloat(float data) {
    Append(&data, sizeof(float ));
}

void Network::Packet::AppendDouble(double data) {
    Append(&data, sizeof(double ));
}
uint8_t Network::Packet::GetUint8() {
    if (BufferOffset + sizeof(uint8_t) > Buffer.size()) {
        return 0;
    }
    uint8_t data = *reinterpret_cast<uint8_t*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(uint8_t);
    return data;
}

uint16_t Network::Packet::GetUint16() {
    if (BufferOffset + sizeof(uint16_t) > Buffer.size()) {
        return 0;
    }
    uint16_t data = *reinterpret_cast<uint16_t*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(uint16_t);
    return data;
}

uint32_t Network::Packet::GetUint32() {
    if (BufferOffset + sizeof(uint32_t) > Buffer.size()) {
        return 0;
    }
    uint32_t data = *reinterpret_cast<uint32_t*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(uint32_t);
    return data;
}

uint64_t Network::Packet::GetUint64() {
    if (BufferOffset + sizeof(uint64_t) > Buffer.size()) {
        return 0;
    }
    uint64_t data = *reinterpret_cast<uint64_t*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(uint64_t);
    return data;
}

std::string Network::Packet::GetString() {
    uint32_t dataSize = GetUint32();
    std::string data;
    if (BufferOffset + dataSize > Buffer.size()) {
        return {};
    }
    data.resize(dataSize);
    data.assign(&Buffer[BufferOffset], dataSize);
    BufferOffset += dataSize;
    return data;
}

bool Network::Packet::GetBool() {
    if (BufferOffset + sizeof(bool) > Buffer.size()) {
        return 0;
    }
    bool data = *reinterpret_cast<bool*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(bool);
    return data;
}

float Network::Packet::GetFloat() {
    if (BufferOffset + sizeof(float) > Buffer.size()) {
        return 0;
    }
    float data = *reinterpret_cast<float*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(float);
    return data;
}

double Network::Packet::GetDouble() {
    if (BufferOffset + sizeof(double) > Buffer.size()) {
        return 0;
    }
    double data = *reinterpret_cast<double*>(&Buffer[BufferOffset]);
    BufferOffset += sizeof(double);
    return data;
}

