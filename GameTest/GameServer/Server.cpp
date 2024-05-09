#include "Server.h"

Server::Server(int port, int maxConnections) {
	this->acceptorPtr = std::make_unique<tcp::acceptor>(this->ioServicePtr, tcp::endpoint(tcp::v4(), port));

	this->maxConnections = maxConnections;
}

void Server::accept() {
	while (true) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(this->ioServicePtr);
		this->acceptorPtr->accept(*socket);
		
		// SERVER FULL
		if (this->usersMap.size() >= this->maxConnections) {
			NetUtils::send_(*socket, NetPacket(NetMessages::SERVER_FULL, nullptr, 0));
			socket.reset();
			continue;
		}
		else {
			NetUtils::send_(*socket, NetPacket(NetMessages::IDLE, nullptr, 0));
		}
		
		std::thread t(&Server::handleClient, this, socket);
		/* with detach the thread code is execute indipendedly */
		t.detach();
	}
}

void Server::handleClient(std::shared_ptr<tcp::socket> socket) {
	std::string nick;
	
	if (!handleUserNickname(socket, nick)) {
		socket->close();
		return;
	}

	NetMessages netMsg;
	while (true) {
		try {
			/* the read continue even after the match request because the client may interrupt the matchmaking */
			netMsg = NetUtils::read_(*socket).getMsgType();
			
			if (netMsg == NetMessages::MATCHMAKING_REQUEST) {
				handleMatchmaking(socket, nick);
				
				/* start the thread to listen if the client wants to undo the matchmaking */
				undoMatchThreadsDeque.push_back(std::thread(&Server::handleUndoMatchmaking, this, socket, nick));
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

void Server::handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string& nick) {
	if (this->matchmakingQueue.empty()) {
		this->matchmakingQueue.push(this->usersMap[nick]);
		NetUtils::send_(*socket, NetPacket(NetMessages::WAIT_FOR_MATCH, nullptr, 0));

		std::cout << "\nClient [nick]: " << nick << " in queue for a match.";
	}
	else {
		// match this client with the last client who requested the match 
		std::shared_ptr<User> player1 = this->matchmakingQueue.front();
		std::shared_ptr<User> player2 = this->usersMap[nick];

		/* send the match found message */
		NetUtils::send_(*player1->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));
		NetUtils::send_(*player2->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));

		this->undoMatchThreadsDeque.back().join();
		this->undoMatchThreadsDeque.pop_back();

		GameSession gameSession(&this->usersMap, player1, player2);
		gameSession.startGame();
	}
}

void Server::handleUndoMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string& nick) {
	using namespace std::chrono_literals;
	NetPacket p;

	socket->non_blocking(true);
	while (true) {
		try {
			std::this_thread::sleep_for(300ms);
	
			p = NetUtils::read_(*socket);
			if (p.getMsgType() == NetMessages::UNDO_MATCHMAKING) {
				this->matchmakingQueue.pop();
				this->usersMap.erase(this->usersMap.find(nick));
				
				std::cout << "\nClient [ " << nick << " ]: " << " undo the matchmaking | Current Players in in Match Queue: " << this->matchmakingQueue.size();
				socket->non_blocking(false);
				return;
			}
			else if (p.getMsgType() == NetMessages::MATCH_FOUND) {
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

bool Server::handleUserNickname(std::shared_ptr<tcp::socket> socket, std::string& nick) {
	try {
		/* read the nickname */
		nick = NetUtils::read_(*socket).getStr();

		// if the nickname already exist 
		if (this->nicknameAlreadyExist(nick)) {
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | refused ( nick already exist )";

			NetUtils::send_(*socket, NetPacket(NetMessages::NICK_EXITS, nullptr, 0));
			return false;
		}
		else {
			/* creates the User */
			this->usersMap[nick] = std::make_shared<User>(nick, socket);
			std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | accepted.";

			NetUtils::send_(*socket, NetPacket(NetMessages::CLIENT_ACCEPTED, nullptr, 0));
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
