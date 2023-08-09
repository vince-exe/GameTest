#include "Server.h"

Server::Server(int port, int maxConnections) {
	this->ioServicePtr = std::make_shared<boost::asio::io_service>();
	this->acceptorPtr = std::make_shared<tcp::acceptor>(*this->ioServicePtr, tcp::endpoint(tcp::v4(), port));

	this->maxConnections = maxConnections;
}

void Server::accept() {
	while (this->usersMap.size() < this->maxConnections) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*this->ioServicePtr);
		this->acceptorPtr->accept(*socket);

		std::thread t([this, socket]() {
			std::shared_ptr<tcp::socket> clientSocket = socket;
			this->handleClient(*clientSocket);
		});
		t.detach();
	}
	std::cout << "\nServer has reached the max connections number ( " << this->maxConnections << " ). [ Stop Listening ].";
}

void Server::handleClient(tcp::socket& socket) {
	std::string nick = NetUtils::read(socket, NetUtils::MSG_DELIMETER);

	/* if the nickname already exist */
	if (this->nicknameAlreadyExist(nick)) {
		std::cout << "\nClient [ IP ]: " << socket.remote_endpoint().address().to_string() << " [ NICK ]: " << nick 
				  << "\nrefused ( nick already exist )";
		socket.close();
		return;
	}
	else {
		usersMap.emplace(nick, User(nick, socket.remote_endpoint().address().to_string()));
		std::cout << "\nClient [ IP ]: " << socket.remote_endpoint().address().to_string() << " [ NICK ]: " << nick << " accepted.";
	}
}

bool Server::nicknameAlreadyExist(const std::string& nick) {
	auto it = usersMap.find(nick);

	return it != usersMap.end();
}
