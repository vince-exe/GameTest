#include "GameSession.h"

GameSession::GameSession(ThreadSafeUnorderedMap<std::string, std::shared_ptr<User>>* usersMap, std::shared_ptr<User> user1, std::shared_ptr<User> user2) {
	m_usersMap = usersMap;
	m_user1 = user1;
	m_user2 = user2;
	m_gameEnd = false;
}

void GameSession::start() {
	/* send the nicknames */
	sendNicknames();
	/* send the positions */
	sendDefaultPositions();

	/* start the game session */
	std::cout << "\nGameSession between " << m_user1->getNick() << " and " << m_user2->getNick() << " started.\n";

	std::thread thUser1(&GameSession::handleClientMessages, this, m_user1, m_user2);
	std::thread thUser2(&GameSession::handleClientMessages, this, m_user2, m_user1);

	/* calculate the damage area's coordinates and send it to the clients */
	setDamageAreasCoordinates();
	/* send the damage areas coordinates and start the game */
	sendDamageAreasConrdinates();

	thUser1.join();
	thUser2.join();

	if (m_gameEnd) {
		handleGameEnd();
	}
	std::cout << "\nGameSession between " << m_user1->getNick() << " and " << m_user2->getNick() << " end.\n";
}

void GameSession::sendNicknames() {
	NetUtils::Tcp::write_(*m_user1->getTCPSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(m_user2->getNick().c_str()), m_user2->getNick().size()));
	NetUtils::Tcp::write_(*m_user2->getTCPSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(m_user1->getNick().c_str()), m_user1->getNick().size()));
}

void GameSession::sendDefaultPositions() {
	float player1Position[] = { 530.f, 470.f };
	float player2Position[] = { 670.f, 470.f };

	NetUtils::Tcp::write_(*m_user1->getTCPSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player1Position), sizeof(player1Position)));
	NetUtils::Tcp::write_(*m_user2->getTCPSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player2Position), sizeof(player2Position)));

	NetUtils::Tcp::write_(*m_user1->getTCPSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player2Position), sizeof(player2Position)));
	NetUtils::Tcp::write_(*m_user2->getTCPSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player1Position), sizeof(player1Position)));
}

void GameSession::setDamageAreasCoordinates() {
	std::random_device rd;
	std::mt19937 gen(rd());

	std::vector<std::pair<float, float>> tempCordsVec;
	std::vector<std::pair<float, float>> areas = {{30.f, 400.f}, {400.f, 800.f}, {800.f, 1130.f}};
	float tmpX, tmpY, k;
	
	for (int i = 0; i < 3; i++) {
		tempCordsVec.clear();
		k = 0;
		/* LEFT PART */
		for (int j = 0; j < 2; j++) {
			std::uniform_real_distribution<float> distrX(areas.at(k).first, areas.at(k).second);
			std::uniform_real_distribution<float> distrY(110, 730);

			tmpX = distrX(gen);
			tmpY = distrY(gen);

			tempCordsVec.push_back(std::pair<float, float>(tmpX, tmpY));
		}
		k++;
		/* CENTRAL PART */
		for (int j = 0; j < 2; j++) {
			std::uniform_real_distribution<float> distrX(areas.at(k).first, areas.at(k).second);
			std::uniform_real_distribution<float> distrY(110, 730);

			tmpX = distrX(gen);
			tmpY = distrY(gen);
			
			if (tmpY >= 335 && tmpY <= 660) {
				j--;
				continue;
			}
			tempCordsVec.push_back(std::pair<float, float>(tmpX, tmpY));
		}
		k++;
		/* RIGTH PART*/
		for (int j = 0; j < 2; j++) {
			std::uniform_real_distribution<float> distrX(areas.at(k).first, areas.at(k).second);
			std::uniform_real_distribution<float> distrY(110, 730);

			tmpX = distrX(gen);
			tmpY = distrY(gen);

			tempCordsVec.push_back(std::pair<float, float>(tmpX, tmpY));
		}
		m_damageAreasCoordinates.push_back(tempCordsVec);
	}
}

void GameSession::sendDamageAreasConrdinates() {
	size_t dataSize;
	std::vector<uint8_t> byteArray = GameSessionUtils::convertCoordinatesToBytes(m_damageAreasCoordinates);
	NetPacket packet(NetPacket::NetMessages::GAME_STARTED, byteArray.data(), byteArray.size());

	NetUtils::Tcp::write_(*m_user1->getTCPSocket(), packet);
	NetUtils::Tcp::write_(*m_user2->getTCPSocket(), packet);
}

void GameSession::handleGameEnd() {
	NetUtils::Tcp::write_(*m_user1->getTCPSocket(), NetPacket(NetPacket::NetMessages::GAME_END, nullptr, 0));
	NetUtils::Tcp::write_(*m_user2->getTCPSocket(), NetPacket(NetPacket::NetMessages::GAME_END, nullptr, 0));

	m_user1->getTCPSocket()->close();
	m_user2->getTCPSocket()->close();
	m_usersMap->erase(m_user1->getNick());
	m_usersMap->erase(m_user2->getNick());
}

void GameSession::handleClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient) {
	NetPacket packet;

	while (true) {
		try {
			packet = NetUtils::Tcp::read_(*client->getTCPSocket());

			if (packet.getMsgType() == NetPacket::NetMessages::GAME_END) {
				m_gameEnd = true;;
				return;
			}

			NetUtils::Tcp::write_(*otherClient->getTCPSocket(), packet);
		}
		catch (const boost::system::system_error& ex) {
			if (otherClient->getTCPSocket()->is_open()) {
				NetUtils::Tcp::write_(*otherClient->getTCPSocket(), NetPacket(NetPacket::NetMessages::QUIT_GAME, nullptr, 0));
			}

			client->getTCPSocket()->close();
			m_usersMap->erase(client->getNick());
			return;
		}
	}
}