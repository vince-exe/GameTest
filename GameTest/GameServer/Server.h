#pragma once

#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "GameSession.h"
#include "network_utilities.h"
#include "User.h"
#include "NetPacket.h"
#include "TemporaryThread.h"
#include "ThreadSafeUnorderedMap.h"
#include "ThreadSafeQueue.h"
#include "TemporaryThreadsManager.h"
#include "hash.h"

using boost::asio::ip::tcp;
using boost::asio::ip::udp;

class Server {
public:
	enum class MatchmakingRequestStates {
		FOUND, 
		WAIT,
		ERR
	};

public:
	Server(unsigned int tcpPort, unsigned int udpPort, unsigned int maxConnections, unsigned int clearUselessThreadsTime, unsigned int udpRequestTimeout, unsigned int threadsNumber);

	void accept();

	void startRoutines();

	ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>>& getUsersMap();

private:
	void handleClient(std::shared_ptr<tcp::socket> socket);
	
	bool nicknameAlreadyExist(const std::string& nick);

	bool handleUserNickname(std::shared_ptr<tcp::socket> socket, const std::string& nick);

	MatchmakingRequestStates handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);

	void handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick);

	void gameSessionThread(const std::string nick);

	void clearUselessThreads();

	void listenUDPConnections();

	bool waitUDPConnection(std::string& nick);

	void processUDPMessages();

private:
	boost::asio::io_service m_ioService;
	std::unique_ptr<tcp::acceptor> m_acceptorPtr;
	std::shared_ptr<udp::socket> m_udpServerSocket;
	
	unsigned int m_maxConnections, m_clearUselessThreadsTime, m_udpPort, m_udpRequestTimeout;
	bool m_doRoutines;

	std::vector<std::thread> m_threadPool;
	std::condition_variable m_threadPoolCv;
	std::mutex m_threadPoolMtx;
	ThreadSafeQueue<std::shared_ptr<NetUdpPacket>> m_udpMessagesQueue;

	ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>> m_usersMap;
	ThreadSafeUnorderedMap<boost::uuids::uuid, std::shared_ptr<GameSession>> m_gameSessionsMap;

	ThreadSafeUnorderedMap<std::string, std::pair<bool, std::shared_ptr<boost::asio::ip::udp::endpoint>>> m_udpConnectionsMap;
	std::condition_variable m_udpConnectionsCv;

	ThreadSafeQueue<std::shared_ptr<User>> m_matchmakingQueue;
	TemporaryThreadsManager m_tempThreadsManager;

	boost::uuids::random_generator m_UUIDGenerator;
};
