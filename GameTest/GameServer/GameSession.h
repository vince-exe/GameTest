#pragma once

#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>

#include "User.h"
#include "network_utilities.h"
#include "NetPacket.h"
#include "Server.h"

using boost::asio::ip::tcp;

class GameSession {
public:
	GameSession(std::unordered_map<std::string, std::shared_ptr<User>>* usersMap, std::shared_ptr<User> user1, std::shared_ptr<User> user2);

	void handleClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient);

	void startGame();

private:
	std::unordered_map<std::string, std::shared_ptr<User>>* usersMap;
	std::shared_ptr<User> user1;
	std::shared_ptr<User> user2;
};

