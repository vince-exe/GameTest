#pragma once

#include <iostream>
#include <unordered_map>
#include <thread>
#include <boost/asio.hpp>

#include "network_utilities.h"
#include "User.h"

using boost::asio::ip::tcp;

class Server {
public:
	Server(int port, int maxConnections);

	void accept();

private:
	void handleClient(tcp::socket& socket);
	
	bool nicknameAlreadyExist(const std::string& nick);

private:
	std::shared_ptr<boost::asio::io_service> ioServicePtr;
	std::shared_ptr<tcp::acceptor> acceptorPtr;

	int maxConnections;
	std::unordered_map<std::string, User> usersMap;
};
