#include "network_utilities.h"

std::string NetUtils::read(tcp::socket& socket, const std::string& until) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, until);
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());

    return data;
}

void NetUtils::send(tcp::socket& socket, const std::string& message, const std::string& delimeter) {
    const std::string msg = message + delimeter;
    boost::asio::write(socket, boost::asio::buffer(msg));
}
