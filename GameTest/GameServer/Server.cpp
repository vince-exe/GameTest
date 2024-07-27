#include "Server.h"

Server::Server(unsigned int tcpPort, unsigned int udpPort, unsigned int maxConnections, unsigned int clearUselessThreadsTime, unsigned int udpRequestTimeout) {
	m_acceptorPtr = std::make_unique<tcp::acceptor>(m_ioService, tcp::endpoint(tcp::v4(), tcpPort));
	m_udpServerSocket = std::make_unique<udp::socket>(m_ioService, udp::endpoint(udp::v4(), udpPort));

	m_udpPort = udpPort;
	m_maxConnections = maxConnections;
	m_doRoutines = true;
	m_clearUselessThreadsTime = clearUselessThreadsTime; 
	m_udpRequestTimeout = udpRequestTimeout;
}

void Server::listenUDPConnections() {
	NetPacket packet;
	std::cout << "\nStarted listening for UDP connections\n";
	while (true) {
		std::shared_ptr<udp::endpoint> remoteEndpoint = std::make_shared<udp::endpoint>(udp::v4(), m_udpPort);
		try {
			packet = NetUtils::Udp::read_(*m_udpServerSocket, *remoteEndpoint);
			std::string nick(reinterpret_cast<const char*>(&packet.getData()[0]), packet.getDataSize());

			m_udpConnectionsMap.insert(nick, std::pair<bool, std::shared_ptr<boost::asio::ip::udp::endpoint>>(true, remoteEndpoint));
			m_udpConnectionsCv.notify_all();
		}
		catch (boost::system::system_error& e) {
			std::cerr << "\nError in listenUDPConnections: " << e.what();
		}
	}
}

bool Server::waitUDPConnection(std::string& nick) {
	std::cout << "\nAspettando una UDP-CONNECTION sul nickname: " << nick;
	bool success;
	std::mutex mutex;

	std::thread waitThread([this, &nick, &success, &mutex] {
		while (true) {
			std::unique_lock<std::mutex> lock(mutex);
			
			if (m_udpConnectionsCv.wait_for(lock, std::chrono::seconds(m_udpRequestTimeout), [this, &nick] { return m_udpConnectionsMap.get(nick).first; })) {
				success = true;
				return;
			}
			else {
				success = false;
				return;
			}
		}
		});
	waitThread.join();

	return success;
}

void Server::accept() {
	while (true) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(m_ioService);
		m_acceptorPtr->accept(*socket);
		
		// SERVER FULL
		if (m_usersMap.size() >= m_maxConnections) {
			NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::SERVER_FULL, nullptr, 0));
			socket.reset();
			continue;
		}
		else {
			NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::IDLE, nullptr, 0));
		}
		
		std::thread t(&Server::handleClient, this, socket);
		/* with detach the thread code is execute indipendedly */
		t.detach();
	}
}

bool Server::handleUserNickname(std::shared_ptr<tcp::socket> socket, const std::string& nick) {
	try {
		// if the nickname already exist 
		if (nicknameAlreadyExist(nick)) {
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | refused ( nick already exist )";

			NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::NICK_EXITS, nullptr, 0));
			return false;
		}
		else {
			// creates the User
			m_usersMap.insert(nick, std::make_shared<User>(nick, socket));
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << "[ " << nick << " ] " << nick << " | accepted.";
			// after this message the client will send the UDP_CONNECTION request
			NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::CLIENT_ACCEPTED, nullptr, 0));
			return true;
		}
	}
	catch (const boost::system::system_error& ex) {
		// temporary catch solution debug 
		std::cout << "\nCatch in handle client... (handleNickname func)";
		m_usersMap.erase(nick);

		return false;
	}
}

void Server::handleClient(std::shared_ptr<tcp::socket> socket) {
	/* read the nickname */
	NetPacket packet = NetUtils::Tcp::read_(*socket);
	std::string nick(reinterpret_cast<const char*>(&packet.getData()[0]), packet.getDataSize());
	
	/* check if there is another user with the same nickname and in case handle it. Otherwise create the user */
	if (!handleUserNickname(socket, nick)) {
		socket->close();
		return;
	}
	m_udpConnectionsMap.insert(nick, std::pair<bool, std::shared_ptr<boost::asio::ip::udp::endpoint>>(false, nullptr));
	
	/* (blocking)waits for x seconds or until a udp connection comes from the client */
	if (waitUDPConnection(nick)) {
		std::cout << "\nConnessione UDP riuscita sul thread: " << nick;
	}
	else {
		std::cout << "\nConnessione UDP non riuscita sul thread: " << nick;
		socket->close();
		m_usersMap.erase(nick);
		m_udpConnectionsMap.erase(nick);
		return;
	}
	
	NetPacket::NetMessages netMsg;
	while (true) {
		try {
			/* the read continue even after the match request because the client may interrupt the matchmaking */
			netMsg = NetUtils::Tcp::read_(*socket).getMsgType();
			
			if (netMsg == NetPacket::NetMessages::MATCHMAKING_REQUEST) {
				if (handleMatchmaking(socket, nick)) {
					std::thread t(&Server::gameSessionThread, this, nick);
					t.detach();
				}
				else {
					/* start the thread to listen if the client wants to undo the matchmaking */
					m_tempThreadsManager.push(TemporaryThread(std::make_shared<std::thread>(&Server::handleUndoMatchmaking, this, socket, nick), false));	
					m_tempThreadsManager.back().getThread()->detach();
				}
				return;
			}
		}
		catch (const boost::system::system_error& ex) {
			std::cerr << "\nCatch in handle client [ Server.cpp ] |" << ex.what() << "\n";
			socket->close();
			m_usersMap.erase(nick);
			m_udpConnectionsMap.erase(nick);

			return;
		}
	}
}

bool Server::handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick) {
	if (m_matchmakingQueue.empty()) {
		m_matchmakingQueue.push(m_usersMap.get(nick));
		NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::WAIT_FOR_MATCH, nullptr, 0));

		std::cout << "\nClient [nick]: " << nick << " in queue for a match.";
		return false;
	}
	return true;
}

void Server::gameSessionThread(const std::string nick) {
	// match this client with the last client who requested the match 
	std::shared_ptr<User> player1 = m_matchmakingQueue.front();
	player1->setUDPEndpoint(m_udpConnectionsMap.get(player1->getNick()).second);
	std::shared_ptr<User> player2 = m_usersMap.get(nick);
	player2->setUDPEndpoint(m_udpConnectionsMap.get(player2->getNick()).second);
	// remove the client from the matchmaking queue because a match has been found
	m_matchmakingQueue.pop();

	/* send the match found message */
	NetUtils::Tcp::write_(*player1->getTCPSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));
	NetUtils::Tcp::write_(*player2->getTCPSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));

	/* delete the thread because the match has been found and it's useless to have. */
	if (m_tempThreadsManager.size()) {
		m_tempThreadsManager.pop();
	}

	Sleep(1000);
	GameSession gameSession(&m_usersMap, player1, player2);
	gameSession.start();
}

//TO-DO: ( REDISIGN ) of this undo-matchmaking-thread
void Server::handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick) {
	using namespace std::chrono_literals;
	NetPacket p;

	socket->non_blocking(true);
	while (true) {
		try {
			std::this_thread::sleep_for(300ms);
	
			p = NetUtils::Tcp::read_(*socket);
			if (p.getMsgType() == NetPacket::NetMessages::UNDO_MATCHMAKING) {
				m_matchmakingQueue.pop();
				m_usersMap.erase(nick);
				m_udpConnectionsMap.erase(nick);
				/* add this thread to the cancellable threads, so it will be deleted */
				m_tempThreadsManager.increaseUselessCounter();

				std::cout << "\nClient [ " << nick << " ]: " << " undo the matchmaking";
				socket->non_blocking(false);
				return;
			}
			else if (p.getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
				socket->non_blocking(false);
				return;
			}
		}
		catch (const boost::system::system_error& e) {
			if (e.code() != boost::asio::error::would_block) {
				std::cerr << "\nCatch in listen for UndoMatchmaking: " << e.what() << std::endl;

				m_matchmakingQueue.pop();
				m_usersMap.erase(nick);
				m_udpConnectionsMap.erase(nick);

				return;
			}
		}
	}
}

bool Server::nicknameAlreadyExist(const std::string& nick) {
	auto it = m_usersMap.find(nick);

	return it != m_usersMap.end();
}

ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>>& Server::getUsersMap() {
	return m_usersMap;
}

void Server::clearUselessThreads() {
	while (m_doRoutines) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(std::chrono::seconds(m_clearUselessThreadsTime));
		
		std::cout << "\n[ LOG ]: Useless Threads Cleared: " << m_tempThreadsManager.clearUselessThreads();
	
	}
	std::cout << "\n[ LOG ]: End ClearUselessThreads routine.";
}

void Server::startRoutines() {
	/* the thread that will host the function to clear useless threads */
	std::thread clearThread(&Server::clearUselessThreads, this);
	clearThread.detach();
	
	/* the thread that listens for udp connections */
	std::thread udpThread(&Server::listenUDPConnections, this);
	udpThread.detach();
}