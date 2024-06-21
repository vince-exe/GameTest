#include "GameSession.h"

GameSession::GameSession(std::unordered_map<std::string, std::shared_ptr<User>>* usersMap, std::shared_ptr<User> user1, std::shared_ptr<User> user2) {
	this->usersMap = usersMap;
	this->user1 = user1;
	this->user2 = user2;
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
	float startX = 38, startY = 117, endX = 1143, endY = 720;
	float randX, randY;

	std::random_device rd;
	std::mt19937 gen(rd());

	std::vector<std::pair<float, float>> vec;
	for (int i = 0; i < 3; i++) {
		vec.clear();
		for (int j = 0; j < 4; j++) {
			std::uniform_real_distribution<float> distrX(startX, endX);
			std::uniform_real_distribution<float> distrY(startY, endY);
			randX = distrX(gen);
			randY = distrY(gen);

			if ((randX >= 400 && randX <= 800) || (randY >= 350 && randY <= 650)) {
				j--;
				continue;
			}
			vec.push_back(std::pair<float, float>(distrX(gen), distrY(gen)));
		}
		damageAreasCoordinates.push_back(vec);
	}
}

void GameSession::sendDamageAreasConrdinates() {
	size_t dataSize;
	std::vector<uint8_t> byteArray = GameSessionUtils::convertCoordinatesToBytes(this->damageAreasCoordinates);
	NetPacket packet(NetPacket::NetMessages::GAME_STARTED, byteArray.data(), byteArray.size());

	NetUtils::write_(*this->user1->getSocket(), packet);
	NetUtils::write_(*this->user2->getSocket(), packet);
}

void GameSession::handleClientMessages(std::shared_ptr<User> client, std::shared_ptr<User> otherClient) {
	NetPacket packet;

	while (true) {
		try {
			packet = NetUtils::read_(*client->getSocket());
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