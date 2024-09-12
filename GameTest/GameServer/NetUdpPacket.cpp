#include "NetUdpPacket.h"

void NetUdpPacket::addCrc32Checksum(std::vector<uint8_t>& data) {
    boost::crc_32_type crc32;

    crc32.process_bytes(data.data(), data.size());
    uint32_t checksum = crc32.checksum();

    uint8_t checksumBytes[4];
    std::memcpy(checksumBytes, &checksum, sizeof(checksum));

    data.insert(data.end(), checksumBytes, checksumBytes + sizeof(checksum));
}

bool NetUdpPacket::verifyCrc32Checksum(const std::vector<uint8_t>& data) {
    if (data.size() < sizeof(uint32_t)) {
        return false;
    }

    boost::crc_32_type crc32;
    crc32.process_bytes(data.data(), data.size() - sizeof(uint32_t));
    uint32_t calculatedChecksum = crc32.checksum();

    uint32_t receivedChecksum = 0;
    std::memcpy(&receivedChecksum, data.data() + data.size() - sizeof(uint32_t), sizeof(uint32_t));

    return (calculatedChecksum == receivedChecksum);
}

NetUdpPacket NetUdpPacket::deserialize(std::vector<uint8_t>& serializedBuff) {
    UdpMessages udpMsg;
    NetPacket::NetMessages gameMsg;
    boost::uuids::uuid sessionId;

    std::memcpy(&udpMsg, serializedBuff.data(), sizeof(UdpMessages));
    std::memcpy(&sessionId, serializedBuff.data() + sizeof(UdpMessages), sizeof(boost::uuids::uuid));
    std::memcpy(&gameMsg, serializedBuff.data() + sizeof(UdpMessages) + sizeof(boost::uuids::uuid), sizeof(NetPacket::NetMessages));

    uint16_t senderLength;
    std::memcpy(&senderLength, serializedBuff.data() + sizeof(UdpMessages) + sizeof(boost::uuids::uuid) + sizeof(NetPacket::NetMessages), sizeof(senderLength));

    std::string sender(reinterpret_cast<const char*>(serializedBuff.data()) + sizeof(UdpMessages) + sizeof(boost::uuids::uuid) + sizeof(NetPacket::NetMessages) + sizeof(senderLength), senderLength);

    size_t packetNumber;
    std::memcpy(&packetNumber, serializedBuff.data() + sizeof(UdpMessages) + sizeof(boost::uuids::uuid) + sizeof(NetPacket::NetMessages) + sizeof(senderLength) + senderLength, sizeof(size_t));

    std::vector<uint8_t> data;
    data.assign(serializedBuff.begin() + sizeof(UdpMessages) + sizeof(boost::uuids::uuid) + sizeof(NetPacket::NetMessages) + sizeof(senderLength) + senderLength + sizeof(packetNumber), serializedBuff.end() - sizeof(uint32_t));

    return NetUdpPacket(sender, udpMsg, sessionId, gameMsg, data.data(), data.size(), packetNumber);
}

NetUdpPacket::NetUdpPacket(std::string& sender, UdpMessages udpMsg, boost::uuids::uuid sessionId, NetPacket::NetMessages gameMsg, const uint8_t* data, size_t size, size_t packetNumber) {
    m_Sender = sender;
    m_UdpMsg = udpMsg;
	m_sessionId = sessionId;
	m_gameMsg = gameMsg;
	m_Data = std::vector<uint8_t>(data, data + size);
	m_Size = size;
    m_packetNumber = packetNumber;
}

NetUdpPacket::NetUdpPacket(NetUdpPacket&& other) noexcept
    : m_UdpMsg(std::move(other.m_UdpMsg)),
    m_gameMsg(std::move(other.m_gameMsg)),
    m_Data(std::move(other.m_Data)),
    m_sessionId(std::move(other.m_sessionId)),
    m_Size(other.m_Size),
    m_packetNumber(other.m_packetNumber)
    {
        other.m_Size = 0;
}

NetUdpPacket& NetUdpPacket::operator=(NetUdpPacket&& other) noexcept {
    if (this != &other) {
        m_UdpMsg = std::move(other.m_UdpMsg);
        m_gameMsg = std::move(other.m_gameMsg);
        m_Data = std::move(other.m_Data);
        m_sessionId = std::move(other.m_sessionId);
        m_Size = other.m_Size;
        m_packetNumber = other.m_packetNumber;
        other.m_Size = 0;
    }
    return *this;
}

size_t NetUdpPacket::size() const {
    return m_Size;
}

NetUdpPacket::UdpMessages NetUdpPacket::getUdpMsg() const {
    return m_UdpMsg;
}

NetPacket::NetMessages NetUdpPacket::getGameMsg() const {
    return m_gameMsg;
}

std::vector<uint8_t>& NetUdpPacket::data() {
    return m_Data;
}

std::vector<uint8_t> NetUdpPacket::serialize() const {
    std::vector<uint8_t> buffer;

    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&m_UdpMsg), reinterpret_cast<const uint8_t*>(&m_UdpMsg) + sizeof(m_UdpMsg));
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&m_sessionId), reinterpret_cast<const uint8_t*>(&m_sessionId) + sizeof(m_sessionId));
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&m_gameMsg), reinterpret_cast<const uint8_t*>(&m_gameMsg) + sizeof(m_gameMsg));
    uint16_t senderLength = static_cast<uint16_t>(m_Sender.size());
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&senderLength), reinterpret_cast<const uint8_t*>(&senderLength) + sizeof(senderLength));
    buffer.insert(buffer.end(), m_Sender.begin(), m_Sender.end());
    buffer.insert(buffer.end(), reinterpret_cast<const uint8_t*>(&m_packetNumber), reinterpret_cast<const uint8_t*>(&m_packetNumber) + sizeof(m_packetNumber));
    buffer.insert(buffer.end(), m_Data.begin(), m_Data.end());
    
    return buffer;
}

void NetUdpPacket::updateData(const uint8_t* data, size_t size, size_t packetNumber) {
    m_packetNumber = packetNumber;
    m_Data = std::vector<uint8_t>(data, data + size);
    m_Size = size;
}

const boost::uuids::uuid& NetUdpPacket::sessionUUID() const {
    return m_sessionId;
}

const std::string& NetUdpPacket::sender() const {
    return m_Sender;
}

const size_t NetUdpPacket::packetNumber() const {
    return m_packetNumber;
}