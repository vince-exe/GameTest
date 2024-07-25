#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <list>
#include <mutex>

#include "GameSession.h"
#include "network_utilities.h"
#include "User.h"
#include "NetPacket.h"
#include "TemporaryThread.h"
#include "ThreadSafeUnorderedMap.h"
#include "ThreadSafeQueue.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Server {
public:
	Server(unsigned int tcpPort, unsigned int udpPort, unsigned int maxConnections, unsigned int clearUselessThreadsTime);

	void accept();

	void startRoutines();

	ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>>& getUsersMap();

private:
	void handleClient(std::shared_ptr<tcp::socket> socket);
	
	bool nicknameAlreadyExist(const std::string& nick);

	bool handleUserNickname(std::shared_ptr<tcp::socket> socket, const std::string& nick);

	// return true if a match has been found
	bool handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);

	void handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);

	void gameSessionThread(const std::string nick);

	void addUselessThread();

	void clearUselessThreads();

	void listenUDPConnections();

	bool waitUDPConnection(std::string& nick);

private:
	boost::asio::io_service m_ioService;
	std::unique_ptr<tcp::acceptor> m_acceptorPtr;
	std::unique_ptr<udp::socket> m_udpServerSocket;

	unsigned int m_maxConnections, m_clearUselessThreadsTime, m_udpPort;
	bool m_doRoutines;

	ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>> m_usersMap;

	ThreadSafeUnorderedMap<std::string, std::pair<bool, std::shared_ptr<boost::asio::ip::udp::endpoint>>> m_udpConnectionsMap;
	std::condition_variable m_udpConnectionsCv;
	std::mutex m_udpConnectionMtx;

	ThreadSafeQueue<std::shared_ptr<User>> m_matchmakingQueue;
	std::list<TemporaryThread> m_tempThreadsList;

	std::mutex m_mtx;
};
