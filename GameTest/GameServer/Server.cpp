#include "Server.h"

Server::Server(int port, int maxConnections) {
	this->ioServicePtr = std::make_shared<boost::asio::io_service>();
	this->acceptorPtr = std::make_shared<tcp::acceptor>(*this->ioServicePtr, tcp::endpoint(tcp::v4(), port));

	this->maxConnections = maxConnections;
}

void Server::accept() {
	int countConnections = 0;
	while (countConnections < this->maxConnections) {
		std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*this->ioServicePtr);
		this->acceptorPtr->accept(*socket);

		std::thread t([this, socket]() {
			std::shared_ptr<tcp::socket> clientSocket = socket;
			this->handleClient(*clientSocket);
		});
		t.detach();
		countConnections++;

		std::cout << "\nConnection ( " << countConnections << " / " << this->maxConnections << " ) accepted";
	}
	std::cout << "\nServer has reached the max connections number ( " << this->maxConnections << " ). [ Stop Listening ].";
}

void Server::handleClient(tcp::socket& socket) {
	NetUtils::send(socket, "HELLO FROM SERVER", NetUtils::MSG_DELIMETER);
}
