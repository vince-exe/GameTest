#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>
#include <thread>

#include "Player.h"
#include "FontManager.h"

class Game {
public:
	enum class GameStates {
		NOT_RUNNING,
		RUNNING,
		END,
	};

private:
	std::vector<std::vector<std::pair<float, float>>> m_damageAreasCoordinates;

	std::atomic<bool> m_blockActions;

	GameStates m_gameState;

	unsigned int m_currentRound, m_waitTimeRound;
	unsigned int m_playerLife, m_enemyLife;

	sf::Vector2f m_startPlayerPosition;
	
public:
	enum class GameEntities {
		PLAYER,
		ENEMY
	};

	enum class GameResults {
		WON,
		LOST
	};

public:
	Game();

	GameStates getGameState();

	GameResults getGameResults();

	void setPlayerStartPosition(sf::Vector2f vec);
	
	sf::Vector2f getStartPlayerPosition();

	unsigned int getCurrentRound();

	unsigned int  getPlayerLife();

	unsigned int getEnemyLife();

	void setBlockActions(bool flag);

	bool areActionsBlocked();

	void setDamageAreasCords(std::vector<std::vector<std::pair<float, float>>> coords);

	void startGame(std::vector<std::vector<sf::CircleShape>>& finalVector);

	bool checkCollision(std::vector<sf::CircleShape> vec, Player& player);

	void handlePlayerMovement(sf::Event& event, Player& player, sf::RenderWindow& window, bool wantSprint);

	/* starts an internal thread that waits for 3 secs ( all the player operations are meanwhile blocked )*/
	void waitRound(sf::Text& text);
	
	/* the entity represents the player that the game has to decrease the life */
	void handleNewRound(GameEntities entity);
};