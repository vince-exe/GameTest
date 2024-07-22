#include <iostream>
#include <boost/asio.hpp>

#include "../GameServer/network_utilities.h"
#include "../GameServer/NetPacket.h"
#include "../Game/network_game_utilities.h"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

int main() {
    // TCP INITIALIZATION 
    boost::asio::io_service io_service;
    std::string nickname;

    tcp::socket socketTCP(io_service);
    boost::system::error_code ec;

    // UDP INITIALIZATION
    udp::resolver resolver(io_service);
    udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "8889");
    udp::socket socketUDP(io_service);
    
    std::cout << "\nNickname: ";
    std::cin >> nickname;

    socketTCP.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888), ec);

    if (ec) {
        std::cerr << "Errore while connecting (TCP): " << ec.message() << std::endl;
        return false;
    }
 
    if(NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::IDLE) {
        NetUtils::Tcp::write_(socketTCP, NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nickname.c_str()), nickname.size()));
        /* send the UDP REQUEST */
        if (NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::CLIENT_ACCEPTED) {
            socketUDP.open(udp::v4());
            boost::system::error_code error;
            if (nickname != "vinforte") {
                NetUtils::Udp::write_(socketUDP, *endpoints.begin(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nickname.c_str()), nickname.size()));
                if (error) {
                    std::cerr << "Error sending message: " << error.message() << std::endl;
                }
                else {
                    std::cout << "Message sent to " << endpoints.begin()->endpoint().address().to_string() << ":" << endpoints.begin()->endpoint().port() << std::endl;
                }
            }
      
        }
    }

    return 0;
}