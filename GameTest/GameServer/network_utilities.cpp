#include "network_utilities.h"

NetPacket NetUtils::Tcp::read_(tcp::socket& socket) {
    size_t packetSize;
    boost::asio::read(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));

    std::vector<uint8_t> receivedData(packetSize);
    boost::asio::read(socket, boost::asio::buffer(receivedData));
    
    return NetPacket::deserialize(receivedData);
}

void NetUtils::Tcp::write_(tcp::socket& socket, const NetPacket& packet) {
    std::vector<uint8_t> serializedData = packet.serialize();
    // calculate the dimension of the packet
    size_t packetSize = serializedData.size();
    
    // send the packet dimension 
    boost::asio::write(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));
    // send the serialized data
    boost::asio::write(socket, boost::asio::buffer(serializedData));
}

NetUdpPacket NetUtils::Udp::read_(udp::socket& socket, udp::endpoint& endpoint) {
    size_t packetSize;
    boost::asio::mutable_buffer size_buffer(&packetSize, sizeof(packetSize));
    socket.receive_from(size_buffer, endpoint);

    // Buffer per i dati ricevuti
    std::vector<uint8_t> receivedData(packetSize);
    boost::asio::mutable_buffer data_buffer(receivedData.data(), packetSize);
    socket.receive_from(data_buffer, endpoint);

    if (!NetUdpPacket::verifyCrc32Checksum(receivedData)) {
        throw std::runtime_error("The data received is corrupted");
    }

    return NetUdpPacket::deserialize(receivedData);
}

void NetUtils::Udp::write_(udp::socket& socket, const udp::endpoint& endpoint, const NetUdpPacket& packet) {
    std::vector<uint8_t> serializedData = packet.serialize();
    NetUdpPacket::addCrc32Checksum(serializedData);
    size_t packetSize = serializedData.size();

    std::vector<uint8_t> sizeBuffer(sizeof(packetSize));
    std::memcpy(sizeBuffer.data(), &packetSize, sizeof(packetSize));

    boost::asio::const_buffer size_buffer = boost::asio::buffer(sizeBuffer);

    boost::system::error_code error_code;
    socket.send_to(size_buffer, endpoint);

    boost::asio::const_buffer data_buffer = boost::asio::buffer(serializedData);
    socket.send_to(data_buffer, endpoint);
}
