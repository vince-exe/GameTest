#pragma once

#include <iostream>
#include <unordered_map>
#include <thread>
#include <boost/asio.hpp>
#include <deque>
#include <queue>

#include "GameSession.h"
#include "network_utilities.h"
#include "User.h"
#include "NetPacket.h"


using boost::asio::ip::tcp;

class Server {
public:
	Server(int port, int maxConnections);

	void accept();

	std::unordered_map<std::string, std::shared_ptr<User>> getUsersMap();

private:
	void handleClient(std::shared_ptr<tcp::socket> socket);
	
	bool nicknameAlreadyExist(const std::string& nick);

	bool handleUserNickname(std::shared_ptr<tcp::socket> socket, std::string& nick);

	void handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string& nick);

	void handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string& nick);

private:
	boost::asio::io_service ioServicePtr;
	std::unique_ptr<tcp::acceptor> acceptorPtr;

	int maxConnections;
	std::unordered_map<std::string, std::shared_ptr<User>> usersMap;

	std::queue<std::shared_ptr<User>> matchmakingQueue;
	std::deque<std::thread> undoMatchThreadsDeque;
};
