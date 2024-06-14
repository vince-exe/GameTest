#include "Server.h"

Server::Server(int port, int maxConnections) {
	this->acceptorPtr = std::make_unique<tcp::acceptor>(this->ioServicePtr, tcp::endpoint(tcp::v4(), port));
	
	TemporaryThread::uselessThreadCounter = 0;
	this->maxConnections = maxConnections;
	this->doRoutines = true;
}

void Server::accept() {
	while (true) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(this->ioServicePtr);
		this->acceptorPtr->accept(*socket);
		
		// SERVER FULL
		if (this->usersMap.size() >= this->maxConnections) {
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
					tempThreadsList.push_back(TemporaryThread(std::make_shared<std::thread>(&Server::handleUndoMatchmaking, this, socket, nick), false));
					tempThreadsList.back().getThread()->detach();
				}
				return;
			}
		}
		catch (const boost::system::system_error& ex) {
			std::cerr << "\nCatch in handle client [ Server.cpp ] |" << ex.what() << "\n";
			socket->close();
			this->usersMap.erase(this->usersMap.find(nick));
			return;
		}
	}
}

bool Server::handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string nick) {
	if (this->matchmakingQueue.empty()) {
		this->matchmakingQueue.push(this->usersMap[nick]);
		NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::WAIT_FOR_MATCH, nullptr, 0));

		std::cout << "\nClient [nick]: " << nick << " in queue for a match.";
		return false;
	}
	return true;
}

void Server::gameSessionThread(const std::string nick) {
	// match this client with the last client who requested the match 
	std::shared_ptr<User> player1 = this->matchmakingQueue.front();
	std::shared_ptr<User> player2 = this->usersMap[nick];

	/* send the match found message */
	NetUtils::write_(*player1->getSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));
	NetUtils::write_(*player2->getSocket(), NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));

	/* delete the thread because the match has been found and it's useless to have. */
	this->mtx.lock();
	if (this->tempThreadsList.size()) {
		this->tempThreadsList.pop_back();
	}
	this->mtx.unlock();

	Sleep(1000);
	GameSession gameSession(&this->usersMap, player1, player2);
	gameSession.startGame();
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
				this->matchmakingQueue.pop();
				this->usersMap.erase(this->usersMap.find(nick));
				/* add this thread to the cancellable threads, so it will be deleted */
				addUselessThread();

				std::cout << "\nClient [ " << nick << " ]: " << " undo the matchmaking | Current Players in in Match Queue: " << this->matchmakingQueue.size();
				socket->non_blocking(false);
				return;
			}
			else if (p.getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
				this->matchmakingQueue.pop();

				socket->non_blocking(false);
				return;
			}
		}
		catch (const boost::system::system_error& e) {
			if (e.code() != boost::asio::error::would_block) {
				std::cerr << "\nCatch in listen for UndoMatchmaking: " << e.what() << std::endl;

				this->matchmakingQueue.pop();
				this->usersMap.erase(this->usersMap.find(nick));
				return;
			}
		}
	}
}

bool Server::handleUserNickname(std::shared_ptr<tcp::socket> socket, const std::string& nick) {
	try {
		// if the nickname already exist 
		if (this->nicknameAlreadyExist(nick)) {
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | refused ( nick already exist )";

			NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::NICK_EXITS, nullptr, 0));
			return false;
		}
		else {
			/* creates the User */
			this->usersMap[nick] = std::make_shared<User>(nick, socket);
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | accepted.";

			NetUtils::write_(*socket, NetPacket(NetPacket::NetMessages::CLIENT_ACCEPTED, nullptr, 0));
			return true;
		}
	}
	catch (const boost::system::system_error& ex) {
		// temporary catch solution debug 
		std::cout << "\nCatch in handle client... (handleNickname func)";
		this->usersMap.erase(this->usersMap.find(nick));

		return false;
	}
}

bool Server::nicknameAlreadyExist(const std::string& nick) {
	auto it = usersMap.find(nick);

	return it != usersMap.end();
}

std::unordered_map<std::string, std::shared_ptr<User>> Server::getUsersMap() {
	return this->usersMap;
}

void Server::addUselessThread() {
	this->mtx.lock();

	TemporaryThread::uselessThreadCounter++;
	int i = 0;
	std::list<TemporaryThread>::iterator it = this->tempThreadsList.begin();

	while (it != this->tempThreadsList.end() && i < TemporaryThread::uselessThreadCounter) {
		it->setCancellable();
		i++;
		it++;
	}
	this->mtx.unlock();
}

void Server::clearUselessThreads() {
	while (this->doRoutines) {
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(15s);

		this->mtx.lock();
		std::list<TemporaryThread>::iterator it = this->tempThreadsList.begin();
		int i = 0;

		while (it != this->tempThreadsList.end() && i < TemporaryThread::uselessThreadCounter) {
			if (it->isCancellable()) {
				it = tempThreadsList.erase(it);
				i++;
				continue;
			}
			it++;
		}
		TemporaryThread::uselessThreadCounter -= i;
		std::cout << "\n[ LOG ]: Useless Threads Cleared: " << i;
		this->mtx.unlock();
	}
	std::cout << "\n[ LOG ]: End ClearUselessThreads routine.";
}

void Server::startRoutines() {
	/* start the thread that will host the function to clear useless threads */
	this->clearThread = std::make_shared<std::thread>(&Server::clearUselessThreads, this);
}
