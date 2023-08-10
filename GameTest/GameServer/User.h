#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class User {
public:
	User(const std::string& nick, std::shared_ptr<tcp::socket> socketPtr);

	const std::string getNick();
	
	const std::string getIp();

	const std::shared_ptr<tcp::socket> getSocket();

private:
	std::string nick;
	std::string ip;
	std::shared_ptr<tcp::socket> socketPtr;
};

