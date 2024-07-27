#include "User.h"

User::User(const std::string& nick, std::shared_ptr<tcp::socket> socketPtr) {
	m_Nick = nick;
	m_Ip = socketPtr->remote_endpoint().address().to_string();
	m_tcpSocket = socketPtr;
	m_udpEndpoint = nullptr;
}

std::string User::getNick() const {
	return m_Nick;
}

std::string User::getIp() const {
	return m_Ip;
}

std::shared_ptr<tcp::socket> User::getTCPSocket() const {
	return m_tcpSocket;
}

void User::setUDPEndpoint(std::shared_ptr<udp::endpoint> endpoint) {
	m_udpEndpoint = endpoint;
}

std::shared_ptr<udp::endpoint> User::getUDPEndpoint() const {
	return m_udpEndpoint;
}