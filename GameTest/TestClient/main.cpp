#include <iostream>
#include <boost/asio.hpp>

#include "../GameServer/network_utilities.h"

using namespace boost::asio;
using ip::tcp;

std::string read(tcp::socket& socket, const std::string& until) {
    boost::asio::streambuf buf;
    boost::asio::read_until(socket, buf, until);
    std::string data = boost::asio::buffer_cast<const char*>(buf.data());

    data.pop_back();

    return data;
}

void send(tcp::socket& socket, const std::string& message, const std::string& delimeter) {
    const std::string msg = message + delimeter;
    boost::asio::write(socket, boost::asio::buffer(msg));
}

int main() {
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    
    std::string nick;
    std::cout << "\nNickname: ";
    std::cin >> nick;

    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888));
    send(socket, nick, NetUtils::MSG_DELIMETER);

    std::string msgServer = read(socket, NetUtils::MSG_DELIMETER);
    std::cout << "\nServer: " << msgServer << "\n";
    if (msgServer == NetMessages::SERVER_FULL) {
        "\nServer Full !";
        socket.close();
        return 0;
    }

    int option;
    while (true) {
        std::cout << "\n1) Matchmaking \
                    \n2) Private Room \
                    \n\n-> ";
        std::cin >> option;

        switch (option) {
        case 1:
            send(socket, NetMessages::MATCHMAKING_REQUEST, NetUtils::MSG_DELIMETER);

            std::cout << "\nServer: " << read(socket, NetUtils::MSG_DELIMETER) << "\n";
            std::cout << "\n5) Exit Matchmaking: ";
            std::cin >> option;

            if (option == 5) {
                send(socket, NetMessages::UNDO_MATCHMAKING, NetUtils::MSG_DELIMETER);
                break;
            }
        }
    }
    system("pause");
    return 0;
}