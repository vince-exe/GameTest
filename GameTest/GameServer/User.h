#pragma once

#include <iostream>

class User {
public:
	User(const std::string& nick, const std::string& ip);

	void setNick(std::string& nick);

	void setIp(std::string& ip);

	std::string getNick();

	std::string getIp();

private:
	std::string nick;
	std::string ip;
};

