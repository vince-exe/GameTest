#pragma once

#include <iostream>
#include <vector>
#include <utility>
#include <SFML/Window.hpp>

#include "../GameServer/NetPacket.h"

namespace NetGameUtils {
	std::string getString(NetPacket& packet);

	sf::Vector2f getSfvector2f(NetPacket& packet);
	
	std::vector<std::vector<std::pair<float, float>>> getDamageAreasCoordinates(NetPacket& packet);
}