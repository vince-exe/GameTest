#include "Client.h"

Client::Client() {
    m_Socket = std::make_shared<tcp::socket>(m_ioService);
    m_udpSocket = std::make_shared<udp::socket>(m_ioService);
    m_udpResolver = std::make_shared<udp::resolver>(m_ioService);
}

bool Client::connect(const std::string& ip, int port) {
    boost::system::error_code ec;
  
    m_Socket->connect(tcp::endpoint(address::from_string(ip), port), ec);
    
    if (ec) {
        std::cerr << "Errore while connecting: " << ec.message() << std::endl;
        return false;
    }
    
    return true;
}

bool Client::resolveUdpEndpoint(const std::string& ip, int port) {
    m_Endpoints = m_udpResolver->resolve(udp::v4(), ip, std::to_string(port));
    if (m_Endpoints.size() != 0) {
        m_Endpoint = m_Endpoints.begin()->endpoint();
        return true;
    }
    return false;
}

std::shared_ptr<tcp::socket> Client::getSocket() {
	return m_Socket;
}

std::shared_ptr<udp::socket> Client::getUdpSocket() {
    return m_udpSocket;
}

udp::endpoint& Client::getUdpEndpoint() {
    return m_Endpoint;
}

void Client::close() {
    if (m_Socket->is_open()) {
        m_Socket->close();
    }
}

void Client::openUdpSocket() {
    if (!m_udpSocket->is_open()) {
        m_udpSocket->open(udp::v4());
    }
}

void Client::closeUdpSocket() {
    if (m_udpSocket->is_open()) {
        m_udpSocket->close();
    }
}