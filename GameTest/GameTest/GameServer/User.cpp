#include "User.h"

User::User(const std::string& nick, std::shared_ptr<tcp::socket> socketPtr) {
	m_Nick = nick;
	m_Ip = socketPtr->remote_endpoint().address().to_string();
	m_socketPtr = socketPtr;
}

const std::string User::getNick() {
	return m_Nick;
}

const std::string User::getIp() {
	return m_Ip;
}

const std::shared_ptr<tcp::socket> User::getSocket() {
	return m_socketPtr;
}
