#include "User.h"

User::User(const std::string& nick, std::shared_ptr<tcp::socket> socketPtr) {
	this->nick = nick;
	this->ip = socketPtr->remote_endpoint().address().to_string();
	this->socketPtr = socketPtr;
}

const std::string User::getNick() {
	return this->nick;
}

const std::string User::getIp() {
	return this->ip;
}

const std::shared_ptr<tcp::socket> User::getSocket() {
	return this->socketPtr;
}
