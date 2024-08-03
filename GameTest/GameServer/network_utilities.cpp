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
    size_t bytes_sent_size = boost::asio::write(socket, boost::asio::buffer(&packetSize, sizeof(packetSize)));
    // send the serialized data
    size_t bytes_sent_data = boost::asio::write(socket, boost::asio::buffer(serializedData));
}

NetPacket NetUtils::Udp::read_(udp::socket& socket, udp::endpoint& endpoint) {
    size_t packetSize;
    boost::asio::mutable_buffer size_buffer(&packetSize, sizeof(packetSize));
    socket.receive_from(size_buffer, endpoint);

    // Buffer per i dati ricevuti
    std::vector<uint8_t> receivedData(packetSize);
    boost::asio::mutable_buffer data_buffer(receivedData.data(), packetSize);
    socket.receive_from(data_buffer, endpoint);

    return NetPacket::deserialize(receivedData);
}

void NetUtils::Udp::write_(udp::socket& socket, const udp::endpoint& endpoint, const NetPacket& packet) {
    std::vector<uint8_t> serializedData = packet.serialize();
    size_t packetSize = serializedData.size();

    // Buffer per la dimensione del pacchetto
    std::vector<uint8_t> sizeBuffer(sizeof(packetSize));
    std::memcpy(sizeBuffer.data(), &packetSize, sizeof(packetSize));

    // Invio della dimensione del pacchetto
    boost::asio::const_buffer size_buffer = boost::asio::buffer(sizeBuffer);
    socket.send_to(size_buffer, endpoint);

    // Invio dei dati serializzati
    boost::asio::const_buffer data_buffer = boost::asio::buffer(serializedData);
    socket.send_to(data_buffer, endpoint);
}
