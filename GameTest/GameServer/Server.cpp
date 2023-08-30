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
		
		std::thread t([this, socket]() {
			/* creates a copy of the socket's pointer to use in the handleClient func */
			this->handleClient(socket);
		});
		t.detach();
	}
}

void Server::handleClient(std::shared_ptr<tcp::socket> socket) {
	std::string nick;
	
	if (!handleUserNickname(socket, nick)) {
		socket->close();
		return;
	}

	// temporary condition ( listen for client's messages )
	NetMessages netMsg;
	while (true) {
		try {
			netMsg = NetUtils::read_(*socket).getMsgType();
			
			if (netMsg == NetMessages::MATCHMAKING_REQUEST) {
				handleMatchmaking(socket, nick);
			}
			else if (netMsg == NetMessages::UNDO_MATCHMAKING) {
				handleUndoMatchmaking(nick);
			}
		}
		catch (const boost::system::system_error& ex) {
			// temporary catch solution debug 
			std::cout << "\nCatch in handle client...";
			socket->close();
			this->usersMap.erase(this->usersMap.find(nick));
			return;
		}
	}
}

bool Server::nicknameAlreadyExist(const std::string& nick) {
	auto it = usersMap.find(nick);

	return it != usersMap.end();
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

void Server::handleMatchmaking(std::shared_ptr<tcp::socket> socket, const std::string& nick) {
	if (this->matchmakingQueue.empty()) {
		this->matchmakingQueue.push(this->usersMap[nick]);
		NetUtils::send_(*socket, NetPacket(NetMessages::WAIT_FOR_MATCH, nullptr, 0));

		std::cout << "\nClient [nick]: " << nick << " in queue for a match!";
	}
	// match this client with the last client who requested the match 
	else {

	}
}

void Server::handleUndoMatchmaking(const std::string& nick) {
	this->matchmakingQueue.pop();
	std::cout << "\nClient [nick]: " << nick << " undo the matchmaking";
}
