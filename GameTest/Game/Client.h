#pragma once

#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio::ip;

class Client  {
private:
	boost::asio::io_service m_ioService;
	std::shared_ptr<tcp::socket> m_Socket;
	std::shared_ptr<udp::socket> m_udpSocket;
	std::shared_ptr<udp::resolver> m_udpResolver;
	udp::resolver::results_type m_Endpoints;
	udp::endpoint m_Endpoint;

public:
	Client();

	bool connect(const std::string& ip, int port);
	
	bool resolveUdpEndpoint(const std::string& ip, int port);

	std::shared_ptr<tcp::socket> getSocket();

	std::shared_ptr<udp::socket> getUdpSocket();

	udp::endpoint& getUdpEndpoint();

	void close();

	void openUdpSocket();

	void closeUdpSocket();
};