#include <iostream>
#include <boost/asio.hpp>

#include "NetPacket.h"

using namespace boost::asio;
using ip::tcp;

NetPacket read_(tcp::socket& socket) {
    try {
        size_t packetSize;
        boost::asio::read(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));

        std::vector<uint8_t> receivedData(packetSize);
        boost::asio::read(socket, boost::asio::buffer(receivedData));

        return NetPacket::deserialize(receivedData);
    }
    catch (const std::exception& e) {
        std::cerr << "Errore durante la ricezione del pacchetto: " << e.what() << std::endl;
        return NetPacket(NetMessages::IDLE, nullptr, 0);
    }
}

void send_(tcp::socket& socket, const NetPacket& packet) {
    try {
        std::vector<uint8_t> serializedData = packet.serialize();

        // Calcola la dimensione del pacchetto
        size_t packetSize = serializedData.size();

        // Invia prima la dimensione del pacchetto
        boost::asio::write(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));

        // Invia quindi i dati del pacchetto
        boost::asio::write(socket, boost::asio::buffer(serializedData));
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);
    
    std::string nick;
    
    std::cout << "\nNickname: ";
    std::cin >> nick;
    
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8888));
    
    if (read_(socket).getMsgType() == NetMessages::SERVER_FULL) {
        std::cout << "\nServer Full!\n";
        socket.close();
        return 0;
    }
    /* send the nickname */
    send_(socket, NetPacket(NetMessages::IDLE, reinterpret_cast<const uint8_t*>(nick.c_str()), nick.size()));
       
    std::cout << "\nNick Size: " << nick.size();
    if (read_(socket).getMsgType() == NetMessages::NICK_EXITS) {
        std::cout << "\nNickname already exists!\n";
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
            send_(socket, NetPacket(NetMessages::MATCHMAKING_REQUEST, nullptr, 0));
            if (read_(socket).getMsgType() == NetMessages::WAIT_FOR_MATCH) {
                std::cout << "\nIn queue for a match!\n";
            }
            std::cout << "\n5) Exit Matchmaking: ";
            std::cin >> option;

            if (option == 5) {
                send_(socket, NetPacket(NetMessages::UNDO_MATCHMAKING, nullptr, 0));
                break;
            }
        }
    }
    
    system("pause");
    return 0;
}