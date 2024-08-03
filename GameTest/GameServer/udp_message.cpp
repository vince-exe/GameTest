#include "udp_message.h"

void UdpMessage::m_serializeString(const std::string& str, std::vector<uint8_t>& buffer) {
    size_t size = str.size();
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&size), reinterpret_cast<const uint8_t*>(&size) + sizeof(size_t));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

std::string UdpMessage::m_deserializeString(const std::vector<uint8_t>& buffer, size_t& offset) {
    size_t size;
    std::memcpy(&size, buffer.data() + offset, sizeof(size_t));
    offset += sizeof(size_t);

    std::string str(buffer.begin() + offset, buffer.begin() + offset + size);
    offset += size;

    return str;
}

std::vector<uint8_t> UdpMessage::serializeUDPMessage(const Message& message) {
    std::vector<uint8_t> buffer;

    m_serializeString(message.m_gameSessionID, buffer);
    m_serializeString(message.m_playerUsername, buffer);

    size_t dataSize = message.data.size();
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&dataSize), reinterpret_cast<const uint8_t*>(&dataSize) + sizeof(size_t));
    buffer.insert(buffer.end(), message.data.begin(), message.data.end());

    return buffer;
}

UdpMessage::Message UdpMessage::deserializeUDPMessage(const std::vector<uint8_t>& buffer) {
    Message message;
    size_t offset = 0;

    message.m_gameSessionID = m_deserializeString(buffer, offset);
    message.m_playerUsername = m_deserializeString(buffer, offset);

    size_t dataSize;
    std::memcpy(&dataSize, buffer.data() + offset, sizeof(size_t));
    offset += sizeof(size_t);

    message.data.insert(message.data.end(), buffer.begin() + offset, buffer.begin() + offset + dataSize);
    offset += dataSize;

    return message;
}
