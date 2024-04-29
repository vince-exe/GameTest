#include "GameSession.h"

GameSession::GameSession(std::shared_ptr<User> user1, std::shared_ptr<User> user2) {
	this->user1 = user1;
	this->user2 = user2;
}

void GameSession::handleClient(std::shared_ptr<User> client, std::shared_ptr<User> otherClient) {
	NetPacket packet;

	while (true) {
		try {
			packet = NetUtils::read_(*client->getSocket());
			NetUtils::send_(*otherClient->getSocket(), packet);
		}
		catch (const boost::system::system_error& ex) {
			std::cerr << "\nCatch in handle client [ " << client->getNick() << " ] GameSession.cpp | " << ex.what();
			return;
		}
	}
}

void GameSession::startGame() {
	/* send the match found message */
	NetUtils::send_(*user1->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));
	NetUtils::send_(*user2->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));
	
	/* send the nicknames */
	NetUtils::send_(*user1->getSocket(), NetPacket(NetMessages::IDLE, reinterpret_cast<const uint8_t*>(user2->getNick().c_str()), user2->getNick().size()));
	NetUtils::send_(*user2->getSocket(), NetPacket(NetMessages::IDLE, reinterpret_cast<const uint8_t*>(user1->getNick().c_str()), user1->getNick().size()));
	
	/* start the game session */
	std::cout << "\nGameSession between " << this->user1->getNick() << " and " << this->user2->getNick() << " started.\n";
	
	std::thread thUser1(&GameSession::handleClient, this, this->user1, this->user2);
	std::thread thUser2(&GameSession::handleClient, this, this->user2, this->user1);

	thUser1.join();
	thUser2.join();

	std::cout << "\nGameSession between " << this->user1->getNick() << " and " << this->user2->getNick() << " end.\n";
}

