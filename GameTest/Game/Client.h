#pragma once

#include <iostream>
#include <boost/asio.hpp>

class Client  {
private:
	boost::asio::io_service m_ioService;
	std::shared_ptr<boost::asio::ip::tcp::socket> m_Socket;

public:
	Client();

	bool connect(const std::string& ip, int port);

	std::shared_ptr<boost::asio::ip::tcp::socket> getSocket();

	void close();
};