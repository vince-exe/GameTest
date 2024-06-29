#include "GameSession.h"

GameSession::GameSession(std::unordered_map<std::string, std::shared_ptr<User>>* usersMap, std::shared_ptr<User> user1, std::shared_ptr<User> user2) {
	this->usersMap = usersMap;
	this->user1 = user1;
	this->user2 = user2;
	m_gameEnd = false;
}

void GameSession::start() {
	/* send the nicknames */
	sendNicknames();
	/* send the positions */
	sendDefaultPositions();

	/* start the game session */
	std::cout << "\nGameSession between " << this->user1->getNick() << " and " << this->user2->getNick() << " started.\n";

	std::thread thUser1(&GameSession::handleClientMessages, this, this->user1, this->user2);
	std::thread thUser2(&GameSession::handleClientMessages, this, this->user2, this->user1);

	/* calculate the damage area's coordinates and send it to the clients */
	setDamageAreasCoordinates();
	/* send the damage areas coordinates and start the game */
	sendDamageAreasConrdinates();

	thUser1.join();
	thUser2.join();

	if (m_gameEnd) {
		handleGameEnd();
	}
	std::cout << "\nGameSession between " << this->user1->getNick() << " and " << this->user2->getNick() << " end.\n";
}

void GameSession::sendNicknames() {
	NetUtils::write_(*user1->getSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(user2->getNick().c_str()), user2->getNick().size()));
	NetUtils::write_(*user2->getSocket(), NetPacket(NetPacket::NetMessages::IDLE, reinterpret_cast<const uint8_t*>(user1->getNick().c_str()), user1->getNick().size()));
}

void GameSession::sendDefaultPositions() {
	float player1Position[] = { 530.f, 470.f };
	float player2Position[] = { 670.f, 470.f };

	NetUtils::write_(*user1->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player1Position), sizeof(player1Position)));
	NetUtils::write_(*user2->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player2Position), sizeof(player2Position)));

	NetUtils::write_(*user1->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player2Position), sizeof(player2Position)));
	NetUtils::write_(*user2->getSocket(), NetPacket(NetPacket::NetMessages::PLAYER_POSITION, reinterpret_cast<const uint8_t*>(player1Position), sizeof(player1Position)));
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
		damageAreasCoordinates.push_back(tempCordsVec);
	}
}

void GameSession::sendDamageAreasConrdinates() {
	size_t dataSize;
	std::vector<uint8_t> byteArray = GameSessionUtils::convertCoordinatesToBytes(this->damageAreasCoordinates);
	NetPacket packet(NetPacket::NetMessages::GAME_STARTED, byteArray.data(), byteArray.size());

	NetUtils::write_(*this->user1->getSocket(), packet);
	NetUtils::write_(*this->user2->getSocket(), packet);
}

void GameSession::handleGameEnd() {
	NetUtils::write_(*user1->getSocket(), NetPacket(NetPacket::NetMessages::GAME_END, nullptr, 0));
	NetUtils::write_(*user2->getSocket(), NetPacket(NetPacket::NetMessages::GAME_END, nullptr, 0));

	user1->getSocket()->close();
	user2->getSocket()->close();
	this->usersMap->erase(this->usersMap->find(user1->getNick()));
	this->usersMap->erase(this->usersMap->find(user2->getNick()));
}

void GameSession::handleClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient) {
	NetPacket packet;

	while (true) {
		try {
			packet = NetUtils::read_(*client->getSocket());

			if (packet.getMsgType() == NetPacket::NetMessages::GAME_END) {
				m_gameEnd = true;;
				return;
			}

			NetUtils::write_(*otherClient->getSocket(), packet);
		}
		catch (const boost::system::system_error& ex) {
			std::cerr << "\nCatch in handle client [ " << client->getNick() << " ] GameSession.cpp | " << ex.what();

			if (otherClient->getSocket()->is_open()) {
				NetUtils::write_(*otherClient->getSocket(), NetPacket(NetPacket::NetMessages::QUIT_GAME, nullptr, 0));
			}

			client->getSocket()->close();
			usersMap->erase(usersMap->find(client->getNick()));
			return;
		}
	}
}