#pragma once

#include <iostream>
#include <boost/asio.hpp>

class Client  {
public:
	Client();

	bool connect(const std::string& ip, int port);

	std::shared_ptr<boost::asio::ip::tcp::socket> getSocket();

	void close();

private:
	boost::asio::io_service io_service;
	std::shared_ptr<boost::asio::ip::tcp::socket> socketPtr;
};

