#include "NetPacket.h"

NetPacket::NetPacket(NetMessages type, const uint8_t* data, size_t dataSize) {
    this->messageType = type;
    this->dataSize = dataSize;
    this->data = std::vector<uint8_t>(data, data + dataSize); // Copia i dati nel vettore
}

NetMessages NetPacket::getMsgType() const {
    return this->messageType;
}

const std::vector<uint8_t>& NetPacket::getData() const {
    return this->data;
}

size_t NetPacket::getDataSize() {
    return this->dataSize;
}

std::vector<uint8_t> NetPacket::serialize() const {
    std::vector<uint8_t> serializedData(sizeof(messageType) + sizeof(size_t) + dataSize);

    std::memcpy(serializedData.data(), &messageType, sizeof(messageType));
    std::memcpy(serializedData.data() + sizeof(messageType), &dataSize, sizeof(size_t));
    std::memcpy(serializedData.data() + sizeof(messageType) + sizeof(size_t), data.data(), dataSize);

    return serializedData;
}

std::string NetPacket::getStr() {
    return std::string(reinterpret_cast<const char*>(&this->data[0]), this->getDataSize());
}

std::vector<int> NetPacket::getIntVec() {
    std::vector<int> intVec(2);

    std::memcpy(&intVec[0], &this->data[0], sizeof(int));
    std::memcpy(&intVec[1], &this->data[sizeof(int)], sizeof(int));

    return intVec;
}

NetPacket NetPacket::deserialize(const std::vector<uint8_t>& serializedData) {
    NetMessages messageType;
    std::memcpy(&messageType, serializedData.data(), sizeof(messageType));

    size_t dataSize;
    std::memcpy(&dataSize, serializedData.data() + sizeof(messageType), sizeof(size_t));

    const uint8_t* dataPtr = serializedData.data() + sizeof(messageType) + sizeof(size_t);

    return NetPacket(messageType, dataPtr, dataSize);
}