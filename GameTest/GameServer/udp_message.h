#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace UdpMessage {
	typedef struct Message {
		std::string m_gameSessionID;
		std::string m_playerUsername;
		std::vector<uint8_t> data;
	}Message;

	void m_serializeString(const std::string& str, std::vector<uint8_t>& buffer);

	std::string m_deserializeString(const std::vector<uint8_t>& buffer, size_t& offset);

	std::vector<uint8_t> serializeUDPMessage(const Message& message);

	Message deserializeUDPMessage(const std::vector<uint8_t>& buffer);
}