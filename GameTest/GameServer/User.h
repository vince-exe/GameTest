#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class User {
private:
	std::string m_Nick;
	std::string m_Ip;
	std::shared_ptr<tcp::socket> m_socketPtr;

public:
	User(const std::string& nick, std::shared_ptr<tcp::socket> socketPtr);

	const std::string getNick();
	
	const std::string getIp();

	const std::shared_ptr<tcp::socket> getSocket();
};
