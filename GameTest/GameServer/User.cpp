#include "User.h"

User::User(const std::string& nick, const std::string& ip) {
	this->nick = nick;
	this->ip = ip;
}

void User::setNick(std::string& nick) {
	this->nick = nick;
}

void User::setIp(std::string& ip) {
	this->ip = ip;
}

std::string User::getNick() {
	return this->nick;
}

std::string User::getIp() {
	return this->ip;
}