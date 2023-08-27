#include "Client.h"

Client::Client() {
	this->socketPtr = std::make_shared<boost::asio::ip::tcp::socket>(io_service);
}

bool Client::connect(const std::string& ip, int port) {
    boost::system::error_code ec;

    try {
        socketPtr->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port), ec);
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Error while connecting: " << e.what() << std::endl;
        return false;
    }

    if (ec) {
        std::cerr << "Errore while connecting: " << ec.message() << std::endl;
        return false;
    }
    
    return true;
}

std::shared_ptr<boost::asio::ip::tcp::socket> Client::getSocket() {
	return this->socketPtr;
}

void Client::close() {
    this->socketPtr->close();
}
