#pragma once

#include <iostream>
#include <boost/asio.hpp>

#include "network_utilities.h"

using boost::asio::ip::tcp;

class Server {
public:
	Server(int64_t port);

	void accept();

private:
	std::shared_ptr<boost::asio::io_service> ioServicePtr;
	std::shared_ptr<tcp::acceptor> acceptorPtr;
};
