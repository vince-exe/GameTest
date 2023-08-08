#include <iostream>
#include <boost/asio.hpp>

#include "../GameServer/network_utilities.h"

using namespace boost::asio;
using ip::tcp;

std::string read(tcp::socket& socket, const std::string& until) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, until);
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());

    return data;
}

void send(tcp::socket& socket, const std::string& message, const std::string& delimeter) {
    const std::string msg = message + delimeter;
    boost::asio::write(socket, boost::asio::buffer(msg));
}

int main() {
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
   
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888));
  
    std::cout << "\nMessaggio Dal Server: " << read(socket, "\n");

    system("pause");
    return 0;
}