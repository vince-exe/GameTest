#pragma once

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class User {
private:
	std::string m_Nick;
	std::string m_Ip;
	std::shared_ptr<tcp::socket> m_tcpSocket;
	std::shared_ptr<udp::endpoint> m_udpEndpoint;

public:
	User(const std::string& nick, std::shared_ptr<tcp::socket> socketPtr);

	std::string getNick() const;
	
	std::string getIp() const;

	std::shared_ptr<tcp::socket> getTCPSocket() const;

	void setUDPEndpoint(std::shared_ptr<udp::endpoint> endpoint);

	std::shared_ptr<udp::endpoint> getUDPEndpoint() const;
};
