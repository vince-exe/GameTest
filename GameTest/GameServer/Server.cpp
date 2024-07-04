#include "Server.h"

Server::Server(int port, int maxConnections) {
	m_acceptorPtr = std::make_unique<tcp::acceptor>(m_ioServicePtr, tcp::endpoint(tcp::v4(), port));
	
	TemporaryThread::uselessThreadCounter = 0;
	m_maxConnections = maxConnections;
	m_doRoutines = true;
}

void Server::accept() {
	while (true) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(m_ioServicePtr);
		m_acceptorPtr->accept(*socket);
		
		// SERVER FULL
		if (m_usersMap.size() >= m_maxConnections) {
			NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::SERVER_FULL, nullptr, 0));
			socket.reset();
			continue;
		}
		else {
			NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::IDLE, nullptr, 0));
		}
		
		std::thread t(&Server::handleClient, this, socket);
		/* with detach the thread code is execute indipendedly */
		t.detach();
	}
}

void Server::handleClient(std::shared_ptr<tcp::socket> socket) {
	/* read the nickname */
	NetPacket packet = NetUtils::read_(*socket);
	std::string nick(reinterpret_cast<const char*>(&packet.getData()[0]), packet.getDataSize());
	
	/* check if there is another user with the same nickname and in case handle it. Otherwise create the user */
	if (!handleUserNickname(socket, nick)) {
		socket->close();
		return;
	}

	NetPacket::NetMessages netMsg;
	while (true) {
		try {
			/* the read continue even after the match request because the client may interrupt the matchmaking */
			netMsg = NetUtils::read_(*socket).getMsgType();
			
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
		NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::WAIT_FOR_MATCH, nullptr, 0));

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
	NetUtils::write_(*player1->getSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));
	NetUtils::write_(*player2->getSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));

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
	
			p = NetUtils::read_(*socket);
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

bool Server::handleUserNickname(std::shared_ptr<tcp::socket> socket, const std::string& nick) {
	try {
		// if the nickname already exist 
		if (nicknameAlreadyExist(nick)) {
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | refused ( nick already exist )";

			NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::NICK_EXITS, nullptr, 0));
			return false;
		}
		else {
			/* creates the User */
			m_usersMap[nick] = std::make_shared<User>(nick, socket);
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | accepted.";

			NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::CLIENT_ACCEPTED, nullptr, 0));
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
		std::this_thread::sleep_for(15s);

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
		std::cout << "\n[ LOG ]: Useless Threads Cleared: " << i;
		m_mtx.unlock();
	}
	std::cout << "\n[ LOG ]: End ClearUselessThreads routine.";
}

void Server::startRoutines() {
	/* start the thread that will host the function to clear useless threads */
	m_clearThread = std::make_shared<std::thread>(&Server::clearUselessThreads, this);
}
