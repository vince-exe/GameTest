#include "network_utilities.h"

NetPacket NetUtils::read_(tcp::socket& socket) {
    size_t packetSize;
    boost::asio::read(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));

    std::vector<uint8_t> receivedData(packetSize);
    boost::asio::read(socket, boost::asio::buffer(receivedData));

    return NetPacket::deserialize(receivedData);
}

bool NetUtils::send_(tcp::socket& socket, const NetPacket& packet) {
    std::vector<uint8_t> serializedData = packet.serialize();
    // calculate the dimension of the packet
    size_t packetSize = serializedData.size();

    // send the packet dimension 
    size_t bytes_sent_size = boost::asio::write(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));
    if (bytes_sent_size != sizeof(packetSize)) {
        return false;
    }

    // send the serialized data
    size_t bytes_sent_data = boost::asio::write(socket, boost::asio::buffer(serializedData));
    if (bytes_sent_data != packetSize) {
        return false;
    }

    return true;
}
