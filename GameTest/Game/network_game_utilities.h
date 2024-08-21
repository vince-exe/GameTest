#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <SFML/Window.hpp>

#include "../GameServer/NetPacket.h"

namespace NetGameUtils {
	std::string getString(const std::vector<uint8_t>& buffer);

	sf::Vector2f getSfvector2f(const std::vector<uint8_t>& buffer);
	
	std::vector<std::vector<std::pair<float, float>>> getDamageAreasCoordinates(const std::vector<uint8_t>& buffer);
}