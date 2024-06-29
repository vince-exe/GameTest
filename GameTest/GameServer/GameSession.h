#pragma once

#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>
#include <random>

#include "User.h"
#include "network_utilities.h"
#include "NetPacket.h"
#include "Server.h"
#include "game_session_utilities.h"

using boost::asio::ip::tcp;

class GameSession {
private:
	std::unordered_map<std::string, std::shared_ptr<User>>* usersMap;
	std::shared_ptr<User> user1;
	std::shared_ptr<User> user2;

	std::vector<std::vector<std::pair<float, float>>> damageAreasCoordinates;
	bool m_gameEnd;
private:
	void handleClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient);
	
	void sendNicknames();

	void sendDefaultPositions();

	void setDamageAreasCoordinates();

	void sendDamageAreasConrdinates();

	void handleGameEnd();

public:
	GameSession(std::unordered_map<std::string, std::shared_ptr<User>>* usersMap, std::shared_ptr<User> user1, std::shared_ptr<User> user2);

	void start();
};

