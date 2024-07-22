#include "Server.h"

Server::Server(unsigned int tcpPort, unsigned int udpPort, unsigned int maxConnections, unsigned int clearUselessThreadsTime) {
	m_acceptorPtr = std::make_unique<tcp::acceptor>(m_ioService, tcp::endpoint(tcp::v4(), tcpPort));
	m_udpServerSocket = std::make_unique<udp::socket>(m_ioService, udp::endpoint(udp::v4(), udpPort));

	TemporaryThread::uselessThreadCounter = 0;
	m_udpPort = udpPort;
	m_maxConnections = maxConnections;
	m_doRoutines = true;
	m_clearUselessThreadsTime = clearUselessThreadsTime;
}

void Server::listenUDPConnections() {
	NetPacket packet;
	std::cout << "\nStarted listening for UDP connections\n";
	while (true) {
		std::shared_ptr<udp::endpoint> remoteEndpoint = std::make_shared<udp::endpoint>(udp::v4(), m_udpPort);
	
		packet = NetUtils::Udp::read_(*m_udpServerSocket, *remoteEndpoint);
		std::string nick(reinterpret_cast<const char*>(&packet.getData()[0]), packet.getDataSize());
		
		{
			std::lock_guard<std::mutex> lock(m_udpConnectionMtx);
			m_udpConnectionsMap[nick] = std::pair<bool, std::shared_ptr<boost::asio::ip::udp::endpoint>>(true, remoteEndpoint);
			// notify all the threads that are waiting that a user does a udp-connection
			m_udpConnectionsCv.notify_all();
		}
	}
}

bool Server::waitUDPConnection(std::string& nick) {
	std::cout << "\nAspettando una UDP-CONNECTION sul nickname: " << nick;
	bool success;

	std::thread waitThread([this, &nick, &success] {
		while (true) {
			std::unique_lock<std::mutex> lock(m_udpConnectionMtx);

			if (m_udpConnectionsCv.wait_for(lock, std::chrono::seconds(20), [this, &nick] { return m_udpConnectionsMap[nick].first; })) {
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
			m_usersMap[nick] = std::make_shared<User>(nick, socket);
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << "[ " << nick << " ] " << nick << " | accepted.";
			//after this message the client will send the UDP_CONNECTION request
			NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::CLIENT_ACCEPTED, nullptr, 0));
			return true;
		}
	}
	catch (const boost::system::system_error& ex) {
		// temporary catch solution debug 
		std::cout << "\nCatch in handle client... (handleNickname func)";
		m_usersMap.erase(m_usersMap.find(nick));

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

	{
		std::lock_guard<std::mutex> lock(m_udpConnectionMtx);
		// set the client udp-connection flag to false and the endpoint to a nullptr.
		m_udpConnectionsMap[nick] = std::pair<bool, std::shared_ptr<boost::asio::ip::udp::endpoint>>(false, nullptr);
	}
	// (blocking) waits for x seconds or until a udp connection comes from the client
	if (waitUDPConnection(nick)) {
		std::cout << "\nConnessione UDP riuscita sul thread: " << nick;
	}
	else {
		std::cout << "\nConnessione UDP non riuscita sul thread: " << nick;
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
					m_tempThreadsList.push_back(TemporaryThread(std::make_shared<std::thread>(&Server::handleUndoMatchmaking, this, socket, nick), false));
					m_tempThreadsList.back().getThread()->detach();
				}
				return;
			}
		}
		catch (const boost::system::system_error& ex) {
			std::cerr << "\nCatch in handle client [ Server.cpp ] |" << ex.what() << "\n";
			socket->close();
			m_usersMap.erase(m_usersMap.find(nick));
			return;
		}
	}
}

bool Server::handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick) {
	if (m_matchmakingQueue.empty()) {
		m_matchmakingQueue.push(m_usersMap[nick]);
		NetUtils::Tcp::write_(*socket, NetPacket(NetPacket::NetMessages::WAIT_FOR_MATCH, nullptr, 0));

		std::cout << "\nClient [nick]: " << nick << " in queue for a match.";
		return false;
	}
	return true;
}

void Server::gameSessionThread(const std::string nick) {
	// match this client with the last client who requested the match 
	std::shared_ptr<User> player1 = m_matchmakingQueue.front();
	std::shared_ptr<User> player2 = m_usersMap[nick];

	/* send the match found message */
	NetUtils::Tcp::write_(*player1->getTCPSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));
	NetUtils::Tcp::write_(*player2->getTCPSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));

	/* delete the thread because the match has been found and it's useless to have. */
	m_mtx.lock();
	if (m_tempThreadsList.size()) {
		m_tempThreadsList.pop_back();
	}
	m_mtx.unlock();

	Sleep(1000);
	GameSession gameSession(&m_usersMap, player1, player2);
	gameSession.start();
}

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
				m_usersMap.erase(m_usersMap.find(nick));
				/* add this thread to the cancellable threads, so it will be deleted */
				addUselessThread();

				std::cout << "\nClient [ " << nick << " ]: " << " undo the matchmaking | Current Players in in Match Queue: " << m_matchmakingQueue.size();
				socket->non_blocking(false);
				return;
			}
			else if (p.getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
				m_matchmakingQueue.pop();

				socket->non_blocking(false);
				return;
			}
		}
		catch (const boost::system::system_error& e) {
			if (e.code() != boost::asio::error::would_block) {
				std::cerr << "\nCatch in listen for UndoMatchmaking: " << e.what() << std::endl;

				m_matchmakingQueue.pop();
				m_usersMap.erase(m_usersMap.find(nick));
				return;
			}
		}
	}
}

bool Server::nicknameAlreadyExist(const std::string& nick) {
	auto it = m_usersMap.find(nick);

	return it != m_usersMap.end();
}

std::unordered_map<std::string, std::shared_ptr<User>> Server::getUsersMap() {
	return m_usersMap;
}

void Server::addUselessThread() {
	m_mtx.lock();

	TemporaryThread::uselessThreadCounter++;
	int i = 0;
	std::list<TemporaryThread>::iterator it = m_tempThreadsList.begin();

	while (it != m_tempThreadsList.end() && i < TemporaryThread::uselessThreadCounter) {
		it->setCancellable();
		i++;
		it++;
	}
	m_mtx.unlock();
}

void Server::clearUselessThreads() {
	while (m_doRoutines) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(std::chrono::seconds(m_clearUselessThreadsTime));
		
		m_mtx.lock();
		std::list<TemporaryThread>::iterator it = m_tempThreadsList.begin();
		int i = 0;

		while (it != m_tempThreadsList.end() && i < TemporaryThread::uselessThreadCounter) {
			if (it->isCancellable()) {
				it = m_tempThreadsList.erase(it);
				i++;
				continue;
			}
			it++;
		}
		TemporaryThread::uselessThreadCounter -= i;
		// TO-DO: use a log system
		if (i > 0) {
			std::cout << "\n[ LOG ]: Useless Threads Cleared: " << i;
		}
		m_mtx.unlock();
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