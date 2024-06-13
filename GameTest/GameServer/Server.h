#pragma once

#include <iostream>
#include <unordered_map>
#include <thread>
#include <boost/asio.hpp>
#include <list>
#include <queue>
#include <mutex>

#include "GameSession.h"
#include "network_utilities.h"
#include "User.h"
#include "NetPacket.h"
#include "TemporaryThread.h"

using boost::asio::ip::tcp;

class Server {
public:
	Server(int port, int maxConnections);

	void accept();

	void startRoutines();

	std::unordered_map<std::string, std::shared_ptr<User>> getUsersMap();

private:
	void handleClient(std::shared_ptr<tcp::socket> socket);
	
	bool nicknameAlreadyExist(const std::string& nick);

	bool handleUserNickname(std::shared_ptr<tcp::socket> socket, std::string& nick);

	/* return true if a match has been found */
	bool handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);

	void handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);
	
	void gameSessionThread(const std::string nick);

	void addUselessThread();

	void clearUselessThreads();

private:
	boost::asio::io_service ioServicePtr;
	std::unique_ptr<tcp::acceptor> acceptorPtr;

	int maxConnections;
	bool doRoutines;

	std::unordered_map<std::string, std::shared_ptr<User>> usersMap;

	std::queue<std::shared_ptr<User>> matchmakingQueue;
	std::list<TemporaryThread> tempThreadsList;

	std::mutex mtx;
	std::shared_ptr<std::thread> clearThread;
};
