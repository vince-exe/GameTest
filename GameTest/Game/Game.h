#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <boost/uuid/uuid.hpp>

#include <iostream>
#include <vector>
#include <utility>
#include <thread>

#include "Player.h"
#include "FontManager.h"
#include "Sound.h"
#include "../GameServer/network_utilities.h"
#include "Client.h"

class Game {
public:
	enum class GameStates {
		NOT_RUNNING,
		RUNNING,
		END,
	};

	typedef struct GameSessionInfo {
		std::string m_playerNick;
		boost::uuids::uuid m_sessionUUID;

	}GameSessionInfo;

private:
	std::vector<std::vector<std::pair<float, float>>> m_damageAreasCoordinates;

	std::atomic<bool> m_blockActions;

	GameStates m_gameState;

	unsigned int m_currentRound, m_waitTimeRound, m_gameMaxTime, m_playerLife, m_enemyLife;

	sf::Vector2f m_startPlayerPosition, m_startEnemyPosition;
	
	bool m_enemyQuit;

public:
	enum class GameEntities {
		PLAYER,
		ENEMY
	};

	enum class GameResults {
		WON,
		LOST,
		DRAW
	};

public:
	Game();

	GameStates getGameState();

	GameResults getGameResults();

	void setPlayerPosition(const sf::Vector2f& vec1);
	
	void setEnemyPosition(const sf::Vector2f& vec2);

	const sf::Vector2f& getStartPlayerPosition() const;

	const sf::Vector2f& getEnemyStartPosition() const;

	unsigned int getCurrentRound();

	unsigned int  getPlayerLife();

	unsigned int getEnemyLife();

	void setBlockActions(bool flag);

	bool areActionsBlocked();

	void setDamageAreasCords(std::vector<std::vector<std::pair<float, float>>> coords);

	void startGame(std::vector<std::vector<sf::CircleShape>>& finalVector);

	bool checkCollision(std::vector<sf::CircleShape> vec, Player& player);

	void startTimer(sf::Text& text);

	void handleEnemyQuit();

	bool hasEnemyQuit();

	/* starts an internal thread that waits for 3 secs ( all the player operations are meanwhile blocked )*/
	void waitRound(sf::Text& text, Sound& soundToPlay);
	
	/* the entity represents the player that the game has to decrease the life */
	void handleNewRound(GameEntities entity);

	void handlePlayerMovement(Player& player, const sf::Vector2f& position, bool wantSprint, Client& client);

	void sendPlayerPosition(const sf::Vector2f& position, float wantSprint, Client& client);

	void handlePlayerCollision(const Player::CollisionSide& collisionSide, Player& player, Client& client);
};