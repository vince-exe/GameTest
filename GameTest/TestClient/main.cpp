#include <iostream>
#include <thread>
#include <boost/asio.hpp>

#include "../GameServer/network_utilities.h"
#include "../GameServer/NetPacket.h"
#include "../Game/network_game_utilities.h"
#include "../GameServer/udp_utilities.h"

using boost::asio::ip::udp;
using boost::asio::ip::tcp;

void readThred(udp::socket& socket, udp::endpoint& endpoint) {
    std::thread t([&socket, &endpoint]() {
        int number = 0;
        while (true) {
            try {
                NetPacket p = NetUtils::Udp::read_(socket, endpoint);
                std::string msgStr(p.getData().begin(), p.getData().end());
                std::cout << "\nMessaggio dall'altro: " << msgStr;
            }
            catch (boost::system::error_code& e) {
                std::cout << "\nerrore in read: " << e.what();
            }
        }
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
    udp::resolver::results_type endpoints = resolver.resolve(udp::v4(), "79.32.155.183", "8889");
    if (endpoints.size() != 0) {
        auto it = endpoints.begin();
        while (it != endpoints.end()) {
            std::cout << "\nendpoint trovato!";
            std::cout << "\nIndirizzo: " << it->endpoint().address() << "  Porta: " << it->endpoint().port();
            it++;
        }
    }
    udp::endpoint endpoint = endpoints.begin()->endpoint();

    udp::socket socketUDP(io_service);
    
    std::cout << "\nNickname: ";
    std::cin >> nickname;
    // SEND THE TCP CONNECTION
    socketTCP.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("79.32.155.183"), 8888), ec);

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

            /* 2.1) CHECK IF THE UDP REQUEST IS OK */
            if (NetUtils::Tcp::read_(socketTCP).getMsgType() != NetPacket::NetMessages::UDP_CONNECTION_SUCCESS) {
                std::cerr << "\nFail della connessione udp";
                socketTCP.close();
                return 0;
            }

            /* 3.) SEND THE MATCHMAKING_REQUEST */
            NetUtils::Tcp::write_(socketTCP, NetPacket(NetPacket::NetMessages::MATCHMAKING_REQUEST, nullptr, 0));

            if (NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::WAIT_FOR_MATCH) {
                std::cout << "\nStarted listening for matchmaking..\n";

                if (NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
                    NetUtils::Tcp::write_(socketTCP, NetPacket(NetPacket::NetMessages::MATCH_FOUND, nullptr, 0));
                    std::cout << "\nMatch trovato mentre stavo aspettando\n";
                }
            }
            else if(NetUtils::Tcp::read_(socketTCP).getMsgType() == NetPacket::NetMessages::MATCH_FOUND) {
                std::cout << "\nMatch trovato\n";
            }

            // 4.) READ THE 3 GAME MESSAGES FROM THE SERVER
            NetUtils::Tcp::read_(socketTCP);
            NetUtils::Tcp::read_(socketTCP);
            NetUtils::Tcp::read_(socketTCP);

            readThred(socketUDP, endpoint);
            std::string msg;
            // ciclo per mandare
  
            while (true) {
                std::cout << "\n-> ";
                std::getline(std::cin, msg);
                UdpUtils::GameMessage message;
                // the combination of nicknames ( it's put like that for testing purposes
                message.m_gameSessionID = "12";
                message.m_playerUsername = nickname;
                message.data = std::vector<uint8_t>(msg.begin(), msg.end());
                std::vector<uint8_t> msgToSend = UdpUtils::serializeUDPMessage(message);
                NetUtils::Udp::write_(socketUDP, endpoint, NetPacket(NetPacket::NetMessages::GAME_UDP_MESSAGE, msgToSend.data(), msgToSend.size()));
            }
        }
    }

    return 0;
}