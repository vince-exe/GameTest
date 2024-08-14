#include "udp_utilities.h"

void UdpUtils::m_serializeString(const std::string& str, std::vector<uint8_t>& buffer) {
    size_t size = str.size();
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&size), reinterpret_cast<const uint8_t*>(&size) + sizeof(size_t));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

std::string UdpUtils::m_deserializeString(const std::vector<uint8_t>& buffer, size_t& offset) {
    size_t size;
    std::memcpy(&size, buffer.data() + offset, sizeof(size_t));
    offset += sizeof(size_t);

    std::string str(buffer.begin() + offset, buffer.begin() + offset + size);
    offset += size;

    return str;
}

void UdpUtils::floatArrToUint8tVec(std::vector<uint8_t>& vec, float* floatArray) {
    uint32_t checksum = 0;
    uint32_t* arrPtr = reinterpret_cast<uint32_t*>(floatArray);

    checksum = arrPtr[0] + arrPtr[1];

    vec.resize(2 * sizeof(float) + sizeof(uint32_t));
    std::memcpy(vec.data(), floatArray, 2 * sizeof(float));
    std::memcpy(vec.data() + 2 * sizeof(float), &checksum, sizeof(uint32_t));
}

bool UdpUtils::uint8tVecToFloatArr(const std::vector<uint8_t>& vec, float* floatArray) {
    if (vec.size() < 2 * sizeof(float) + sizeof(uint32_t)) {
        return false;
    }
    std::memcpy(&floatArray[0], vec.data(), sizeof(float));       
    std::memcpy(&floatArray[1], vec.data() + sizeof(float), sizeof(float));

    uint32_t checksum = 0;
    std::memcpy(&checksum, vec.data() + 2 * sizeof(float), sizeof(uint32_t));

    uint32_t* ptr = reinterpret_cast<uint32_t*>(floatArray);
    uint32_t calculatedChecksum = ptr[0] + ptr[1];

    return (calculatedChecksum == checksum);
}

std::vector<uint8_t> UdpUtils::serializeUDPMessage(const GameMessage& message) {
    std::vector<uint8_t> buffer;

    const uint8_t* uuid = message.m_gameSessionID.data;
    buffer.insert(buffer.end(), uuid, uuid + 16);
    m_serializeString(message.m_playerUsername, buffer);

    size_t dataSize = message.data.size();
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&dataSize), reinterpret_cast<const uint8_t*>(&dataSize) + sizeof(size_t));
    buffer.insert(buffer.end(), message.data.begin(), message.data.end());

    return buffer;
}

UdpUtils::GameMessage UdpUtils::deserializeUDPMessage(const std::vector<uint8_t>& buffer) {
    GameMessage message;
    size_t offset = 0;

    std::copy(buffer.begin(), buffer.begin() + 16, message.m_gameSessionID.begin());
    offset += 16;

    message.m_playerUsername = m_deserializeString(buffer, offset);

    size_t dataSize;
    std::memcpy(&dataSize, buffer.data() + offset, sizeof(size_t));
    offset += sizeof(size_t);

    message.data.insert(message.data.end(), buffer.begin() + offset, buffer.begin() + offset + dataSize);
    offset += dataSize;

    return message;
}
