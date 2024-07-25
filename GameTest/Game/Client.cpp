#include "Client.h"

Client::Client() {
    m_Socket = std::make_shared<boost::asio::ip::tcp::socket>(m_ioService);
}

bool Client::connect(const std::string& ip, int port) {
    boost::system::error_code ec;
  
    m_Socket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port), ec);
    
    if (ec) {
        std::cerr << "Errore while connecting: " << ec.message() << std::endl;
        return false;
    }
    
    return true;
}

std::shared_ptr<boost::asio::ip::tcp::socket> Client::getSocket() {
	return m_Socket;
}

void Client::close() {
    if (m_Socket->is_open()) {
        m_Socket->close();
    }
}