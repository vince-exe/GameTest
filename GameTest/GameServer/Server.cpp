#include "Server.h"

Server::Server(int64_t port) {
	this->ioServicePtr = std::make_shared<boost::asio::io_service>();
	this->acceptorPtr = std::make_shared<tcp::acceptor>(*this->ioServicePtr, tcp::endpoint(tcp::v4(), port));
}

void Server::accept() {
	std::shared_ptr<tcp::socket> socket = std::make_shared<tcp::socket>(*this->ioServicePtr);
	this->acceptorPtr->accept(*socket);

	std::string testMsg = "HELLO FROM SERVER";
	NetUtils::send(*socket, testMsg, NetUtils::MSG_DELIMETER);
}