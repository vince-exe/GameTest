#pragma once

#include <SFML/Window.hpp>

#include "../GameServer/NetPacket.h"

namespace NetGameUtils {
	std::string getString(NetPacket& packet);

	sf::Vector2f getSfvector2f(NetPacket& packet);
}