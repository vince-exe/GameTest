#include "game_session_utilities.h"

std::vector<uint8_t> GameSessionUtils::convertCoordinatesToBytes(const std::vector<std::vector<std::pair<float, float>>>& data) {
    // Calculate the total size needed
    size_t totalSize = sizeof(size_t); // For storing the size of the outer vector
    for (const auto& innerVec : data) {
        totalSize += sizeof(size_t); // For storing sizes of inner vectors
        totalSize += innerVec.size() * sizeof(std::pair<float, float>);
    }

    // Allocate a vector to hold the serialized data
    std::vector<uint8_t> buffer(totalSize);
    uint8_t* ptr = buffer.data();

    // Serialize the outer vector size
    size_t outerSize = data.size();
    std::memcpy(ptr, &outerSize, sizeof(size_t));
    ptr += sizeof(size_t);

    // Serialize the data
    for (const auto& innerVec : data) {
        size_t innerSize = innerVec.size();
        std::memcpy(ptr, &innerSize, sizeof(size_t));
        ptr += sizeof(size_t);

        for (const auto& pair : innerVec) {
            std::memcpy(ptr, &pair.first, sizeof(float));
            ptr += sizeof(float);
            std::memcpy(ptr, &pair.second, sizeof(float));
            ptr += sizeof(float);
        }
    }

    return buffer;
}