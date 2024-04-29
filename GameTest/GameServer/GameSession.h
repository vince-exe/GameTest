#pragma once

#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>

#include "User.h"
#include "network_utilities.h"
#include "NetPacket.h"

using boost::asio::ip::tcp;

class GameSession {
public:
	GameSession(std::shared_ptr<User> user1, std::shared_ptr<User> user2);

	void handleClient(std::shared_ptr<User> client, std::shared_ptr<User> otherClient);

	void startGame();

private:
	std::shared_ptr<User> user1;
	std::shared_ptr<User> user2;
};

