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

	bool handleUserNickname(std::shared_ptr<tcp::socket> socket, const std::string& nick);

	/* return true if a match has been found */
	bool handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);

	void handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);
	
	void gameSessionThread(const std::string nick);

	void addUselessThread();

	void clearUselessThreads();

private:
	boost::asio::io_service m_ioServicePtr;
	std::unique_ptr<tcp::acceptor> m_acceptorPtr;

	int m_maxConnections;
	bool m_doRoutines;

	std::unordered_map<std::string, std::shared_ptr<User>> m_usersMap;

	std::queue<std::shared_ptr<User>> m_matchmakingQueue;
	std::list<TemporaryThread> m_tempThreadsList;

	std::mutex m_mtx;
	std::shared_ptr<std::thread> m_clearThread;
};
