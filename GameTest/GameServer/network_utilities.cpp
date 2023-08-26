#include "network_utilities.h"

NetPacket NetUtils::read_(tcp::socket& socket) {
    try {
        size_t packetSize;
        boost::asio::read(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));

        std::vector<uint8_t> receivedData(packetSize);
        boost::asio::read(socket, boost::asio::buffer(receivedData));

        return NetPacket::deserialize(receivedData);
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void NetUtils::send_(tcp::socket& socket, const NetPacket& packet) {
    try {
        std::vector<uint8_t> serializedData = packet.serialize();

        // calculate the dimension of the packet
        size_t packetSize = serializedData.size();

        // send packet dimension
        boost::asio::write(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));

        // send datas
        boost::asio::write(socket, boost::asio::buffer(serializedData));
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
