#pragma once
#include "Core.h"
#include <vector>
#include <string>

#define MAX_PACKET_SIZE 16000
namespace Network{
    class Packet {
    public:
        explicit Packet(uint16_t type = 0);
        void Clear();
        void SetPacketType(uint16_t type);
        uint16_t GetPacketType() const;
        bool IsEmpty();
        void AppendUint8(uint8_t data);
        void AppendUint16(uint16_t data);
        void AppendUint32(uint32_t data);
        void AppendUint64(uint64_t data);
        void AppendString(const std::string& data);
        void AppendBool(bool data);
        void AppendFloat(float data);
        void AppendDouble(double data);
        uint8_t GetUint8();
        uint16_t GetUint16();
        uint32_t GetUint32();
        uint64_t GetUint64();
        std::string GetString();
        bool GetBool();
        float GetFloat();
        double GetDouble();

        std::vector<char> Buffer;
        uint32_t BufferOffset = 0;

    private:
        void Append(const void* data, uint32_t size);
    };
}
