#pragma once

#include <iostream>
#include <unordered_map>
#include <boost/asio.hpp>
#include <random>

#include "User.h"
#include "network_utilities.h"
#include "NetPacket.h"
#include "game_session_utilities.h"
#include "ThreadSafeUnorderedMap.h"
#include "ThreadSafeQueue.h"
#include "udp_message.h"

using boost::asio::ip::tcp;

class GameSession {
private:
	ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>>* m_usersMap;

	std::shared_ptr<udp::socket> m_udpServerSocket;
	std::shared_ptr<User> m_user1;
	std::shared_ptr<User> m_user2;

	std::vector<std::vector<std::pair<float, float>>> m_damageAreasCoordinates;
	bool m_gameEnd;
private:
	void handleClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient);
	
	void handleUDPClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient);

	void sendNicknames();

	void sendDefaultPositions();

	void setDamageAreasCoordinates();

	void sendDamageAreasConrdinates();

	void handleGameEnd();

public:
	GameSession(ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>>* usersMap, std::shared_ptr<User> user1, std::shared_ptr<User> user2, std::shared_ptr<udp::socket> serverSocket);

	void handleUDPMessage(UdpMessage::Message& message,std::shared_ptr<NetPacket> packet);

	void start();
};

