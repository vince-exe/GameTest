#include "network_game_utilities.h"

std::string NetGameUtils::getString(const std::vector<uint8_t>& buffer) {
    return std::string(reinterpret_cast<const char*>(&buffer[0]), buffer.size());
}

sf::Vector2f NetGameUtils::getSfvector2f(const std::vector<uint8_t>& buffer) {
    std::vector<float> floatVec(2);

    std::memcpy(&floatVec[0], &buffer[0], sizeof(float));
    std::memcpy(&floatVec[1], &buffer[sizeof(float)], sizeof(float));

    return sf::Vector2f(floatVec[0], floatVec[1]);
}

std::vector<std::vector<std::pair<float, float>>> NetGameUtils::getDamageAreasCoordinates(const std::vector<uint8_t>& buffer) {
    const uint8_t* ptr = buffer.data();
    const uint8_t* end = ptr + buffer.size();

    std::vector<std::vector<std::pair<float, float>>> result;

    // Read the outer vector size
    size_t outerSize;
    std::memcpy(&outerSize, ptr, sizeof(size_t));
    ptr += sizeof(size_t);

    for (size_t i = 0; i < outerSize; ++i) {
        // Read the size of the inner vector
        size_t innerSize;
        std::memcpy(&innerSize, ptr, sizeof(size_t));
        ptr += sizeof(size_t);

        std::vector<std::pair<float, float>> innerVec;
        innerVec.reserve(innerSize);

        for (size_t j = 0; j < innerSize; ++j) {
            float first, second;
            std::memcpy(&first, ptr, sizeof(float));
            ptr += sizeof(float);
            std::memcpy(&second, ptr, sizeof(float));
            ptr += sizeof(float);
            innerVec.emplace_back(first, second);
        }

        result.push_back(innerVec);
    }

    return result;
}