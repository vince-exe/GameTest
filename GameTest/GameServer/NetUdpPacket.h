#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/crc.hpp>

#include "NetPacket.h"

class NetUdpPacket {
public:
	enum class UdpMessages {
		CONNECTION_MESSAGE,
		GAME_MESSAGE
	};

private:
	// HEADER
	UdpMessages m_UdpMsg;
	NetPacket::NetMessages m_gameMsg;
	boost::uuids::uuid m_sessionId;
	std::string m_Sender;
	size_t m_Size;

	// PAYLOAD
	std::vector<uint8_t> m_Data;

public:
	static void addCrc32Checksum(std::vector<uint8_t>& data);

	static bool verifyCrc32Checksum(const std::vector<uint8_t>& data);

	static NetUdpPacket deserialize(std::vector<uint8_t>& serializedBuff);

public:
	NetUdpPacket() = default;

	NetUdpPacket(std::string& sender, UdpMessages udpMsg, boost::uuids::uuid sessionId, NetPacket::NetMessages gameMsg, const uint8_t* data, size_t size);

	NetUdpPacket(const NetUdpPacket& other) = default;

	NetUdpPacket& operator=(const NetUdpPacket& other) = default;

	NetUdpPacket(NetUdpPacket&& other) noexcept;

	NetUdpPacket& operator=(NetUdpPacket&& other) noexcept;

	size_t size() const;

	UdpMessages getUdpMsg() const;

	NetPacket::NetMessages getGameMsg() const;

	std::vector<uint8_t>& data();

	std::vector<uint8_t> serialize() const;

	const boost::uuids::uuid& sessionUUID() const;

	const std::string& sender() const;
};