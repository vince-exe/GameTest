#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "../GameServer/network_utilities.h"
#include "../GameServer/NetPacket.h"
#include "../Game/network_game_utilities.h"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

void readThred(udp::socket& socket, udp::endpoint& endpoint) {
    NetPacket p;
    std::thread t([&socket, &p, &endpoint]() {
        p = NetUtils::Udp::read_(socket, endpoint);

        std::string msg(reinterpret_cast<const char*>(&p.getData()[0]), p.getDataSize());
        std::cout << "\nMessaggio dall'altro: " << msg;
    });
    t.detach();
}

int main() {
    // TCP INITIALIZATION 
    boost::asio::io_service io_service;
    std::string nickname;

    tcp::socket socketTCP(io_service);
    boost::system::error_code ec;

    // UDP INITIALIZATION
    udp::resolver resolver(io_service);
    udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "127.0.0.1", "8889");
    udp::endpoint endpoint = endpoints.begin()->endpoint();

    udp::socket socketUDP(io_service);
    
    std::cout << "\nNickname: ";
    std::cin >> nickname;
    // SEND THE TCP CONNECTION
    socketTCP.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888), ec);

    if (ec) {
        std::cerr << "Errore while connecting (TCP): " << ec.message() << std::endl;
        return false;
    }
    
    if(NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::IDLE) {
        // 1.) SEND THE NICKNAME
        NetUtils::Tcp::write_(socketTCP, NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nickname.c_str()), nickname.size()));
       
        if (NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::CLIENT_ACCEPTED) {
            socketUDP.open(udp::v4());
            boost::system::error_code error;
            /* 2.) SEND THE UDP REQUEST */
            NetUtils::Udp::write_(socketUDP, endpoint, NetPacket(NetPacket::NetMessages::CONNECTION_UDP_MESSAGE, reinterpret_cast<const uint8_t*>(nickname.c_str()), nickname.size()));

            /* 3.) SEND THE MATCHMAKING_REQUEST */
            NetUtils::Tcp::write_(socketTCP, NetPacket(NetPacket::NetMessages::MATCHMAKING_REQUEST, nullptr, 0));

            if (NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::WAIT_FOR_MATCH) {
                std::cout << "\nStarted listening for matchmaking..\n";

                if (NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
                    std::cout << "\nMatch trovato\n";
                }
            }
            else if(NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
                std::cout << "\nMatch trovato\n";
            }

            Sleep(1000);
            // 4.) READ THE 3 GAME MESSAGES FROM THE SERVER
            NetUtils::Tcp::read_(socketTCP);
            NetUtils::Tcp::read_(socketTCP);
            NetUtils::Tcp::read_(socketTCP);

            readThred(socketUDP, endpoint);
            std::string msg;
            // ciclo per mandare
            while (true) {
                std::cout << "\nMessaggio: ";
                std::cin >> msg;

                NetUtils::Udp::write_(socketUDP, endpoint, NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(msg.c_str()), msg.size()));
            }
        }
    }

    return 0;
}