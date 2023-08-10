#include "Server.h"

Server::Server(int port, int maxConnections) {
	this->acceptorPtr = std::make_unique<tcp::acceptor>(this->ioServicePtr, tcp::endpoint(tcp::v4(), port));

	this->maxConnections = maxConnections;
}

void Server::accept() {
	while (true) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(this->ioServicePtr);
		this->acceptorPtr->accept(*socket);

		if (this->usersMap.size() >= this->maxConnections) {
			NetUtils::send(*socket, NetMessages::SERVER_FULL, NetUtils::MSG_DELIMETER);
			socket.reset();
			continue;
		}

		std::thread t([this, socket]() {
			/* creates a copy of the socket's pointer to use in the handleClient func */
			this->handleClient(socket);
		});
		t.detach();
	}
}

void Server::handleClient(std::shared_ptr<tcp::socket> socket) {
	/* take the nickname */
	std::string nick = NetUtils::read(*socket, NetUtils::MSG_DELIMETER);

	/* if the nickname already exist */
	if (this->nicknameAlreadyExist(nick)) {
		std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | refused ( nick already exist )";

		NetUtils::send(*socket, NetMessages::NICK_ALREADY_EXIST, NetUtils::MSG_DELIMETER);
		socket->close();
		return;
	}
	else {
		this->usersMap.emplace(nick, User(nick, socket));
		std::cout << "\nClient [ IP ]: " << socket->remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " | accepted.";
	
		NetUtils::send(*socket, NetMessages::CLIENT_ACCEPTED, NetUtils::MSG_DELIMETER);
	}

	/* temporary condition ( listen for client's messages )*/
	std::string msg;
	while (true) {
		try {
			msg = NetUtils::read(*socket, NetUtils::MSG_DELIMETER);
			
			if (msg == NetMessages::MATCHMAKING_REQUEST) {
				/* match this client with the last client who requested the match */
			}
		}
		catch (const boost::system::system_error& ex) {
			/* temporary catch solution debug */
			std::cout << "\nError in handle client...";
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
