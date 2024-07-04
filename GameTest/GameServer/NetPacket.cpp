#include "NetPacket.h"

std::vector<uint8_t> NetPacket::serialize() const {
    std::vector<uint8_t> serializedData(sizeof(m_messageType) + sizeof(size_t) + m_dataSize);

    std::memcpy(serializedData.data(), &m_messageType, sizeof(m_messageType));
    std::memcpy(serializedData.data() + sizeof(m_messageType), &m_dataSize, sizeof(size_t));
    std::memcpy(serializedData.data() + sizeof(m_messageType) + sizeof(size_t), m_Data.data(), m_dataSize);

    return serializedData;
}

NetPacket NetPacket::deserialize(const std::vector<uint8_t>& serializedData) {
    NetMessages m_messageType;
    std::memcpy(&m_messageType, serializedData.data(), sizeof(m_messageType));

    size_t m_DataSize;
    std::memcpy(&m_DataSize, serializedData.data() + sizeof(m_messageType), sizeof(size_t));

    const uint8_t* m_DataPtr = serializedData.data() + sizeof(m_messageType) + sizeof(size_t);

    return NetPacket(m_messageType, m_DataPtr, m_DataSize);
}

NetPacket::NetPacket(NetMessages type, const uint8_t* data_, size_t dataSize_) {
    m_messageType = type;
    m_dataSize = dataSize_;
    m_Data = std::vector<uint8_t>(data_, data_ + m_dataSize);
}

NetPacket::NetMessages NetPacket::getMsgType() const {
    return m_messageType;
}

const std::vector<uint8_t>& NetPacket::getData() const {
    return m_Data;
}

size_t NetPacket::getDataSize() {
    return m_dataSize;
}