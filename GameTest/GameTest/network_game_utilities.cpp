#include "network_game_utilities.h"

std::string NetGameUtils::getString(NetPacket& packet) {
    return std::string(reinterpret_cast<const char*>(&packet.getData()[0]), packet.getDataSize());
}

sf::Vector2f NetGameUtils::getSfvector2f(NetPacket& packet) {
    std::vector<float> floatVec(2);

    std::memcpy(&floatVec[0], &packet.getData()[0], sizeof(float));
    std::memcpy(&floatVec[1], &packet.getData()[sizeof(float)], sizeof(float));

    return sf::Vector2f(floatVec[0], floatVec[1]);
}