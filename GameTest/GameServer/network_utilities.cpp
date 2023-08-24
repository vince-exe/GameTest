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
        std::cerr << "Errore durante la ricezione del pacchetto: " << e.what() << std::endl;
        // Restituisci un oggetto NetPacket vuoto o un oggetto con un messaggio di errore
    }
}

void NetUtils::send_(tcp::socket& socket, const NetPacket& packet) {
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
