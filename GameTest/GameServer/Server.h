#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <boost/asio.hpp>

#include "network_utilities.h"

using boost::asio::ip::tcp;

class Server {
public:
	Server(int port, int maxConnections);

	void accept();

private:
	void handleClient(tcp::socket& socket);

private:
	std::shared_ptr<boost::asio::io_service> ioServicePtr;
	std::shared_ptr<tcp::acceptor> acceptorPtr;

	int maxConnections;
	std::vector<std::thread> threads;
};
