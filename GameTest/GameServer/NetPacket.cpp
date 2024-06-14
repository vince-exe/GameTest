#include "NetPacket.h"

std::vector<uint8_t> NetPacket::serialize() const {
    std::vector<uint8_t> serializedData(sizeof(messageType) + sizeof(size_t) + dataSize);

    std::memcpy(serializedData.data(), &messageType, sizeof(messageType));
    std::memcpy(serializedData.data() + sizeof(messageType), &dataSize, sizeof(size_t));
    std::memcpy(serializedData.data() + sizeof(messageType) + sizeof(size_t), data.data(), dataSize);

    return serializedData;
}

NetPacket NetPacket::deserialize(const std::vector<uint8_t>& serializedData) {
    NetMessages messageType;
    std::memcpy(&messageType, serializedData.data(), sizeof(messageType));

    size_t dataSize;
    std::memcpy(&dataSize, serializedData.data() + sizeof(messageType), sizeof(size_t));

    const uint8_t* dataPtr = serializedData.data() + sizeof(messageType) + sizeof(size_t);

    return NetPacket(messageType, dataPtr, dataSize);
}

NetPacket::NetPacket(NetMessages type, const uint8_t* data, size_t dataSize) {
    this->messageType = type;
    this->dataSize = dataSize;
    this->data = std::vector<uint8_t>(data, data + dataSize);
}

NetPacket::NetMessages NetPacket::getMsgType() const {
    return this->messageType;
}

const std::vector<uint8_t>& NetPacket::getData() const {
    return this->data;
}

size_t NetPacket::getDataSize() {
    return this->dataSize;
}