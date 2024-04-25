#include "GameSession.h"

GameSession::GameSession(std::shared_ptr<User> user1, std::shared_ptr<User> user2) {
	this->user1 = user1;
	this->user2 = user2;
}

void GameSession::startGame() {
	/* send the match found message */
	NetUtils::send_(*user1->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));
	NetUtils::send_(*user2->getSocket(), NetPacket(NetMessages::MATCH_FOUND, nullptr, 0));
	
	/* send the nicknames */
	NetUtils::send_(*user1->getSocket(), NetPacket(NetMessages::MATCH_FOUND, reinterpret_cast<const uint8_t*>(user2->getNick().c_str()), user2->getNick().size()));
	NetUtils::send_(*user2->getSocket(), NetPacket(NetMessages::MATCH_FOUND, reinterpret_cast<const uint8_t*>(user1->getNick().c_str()), user1->getNick().size()));
	
	/* start the game session */
	std::cout << "\nMatchmaking between " << this->user1->getNick() << " and " << this->user2->getNick() << " started.\n";
	
	while (true) {
		;
	}
}

