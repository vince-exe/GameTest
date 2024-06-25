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
private:
	std::vector<std::vector<std::pair<float, float>>> m_damageAreasCoordinates;
	std::vector<sf::CircleShape> m_damageAreasVector;

	std::atomic<bool> m_blockActions;
	bool m_gameStarted;

	unsigned int m_currentRound, m_waitTimeRound;
	unsigned int m_playerLife, m_enemyLife;

	sf::Vector2f m_startPlayerPosition;

	sf::Text m_waitRoundText;

public:
	Game();

	void initSprites(sf::RenderWindow& window);

	void drawDamageAreasShapes(sf::RenderWindow& window);

	void drawWaitRoundText(sf::RenderWindow& window);

	void setPlayerStartPosition(sf::Vector2f vec);
	
	sf::Vector2f getStartPlayerPosition();

	unsigned int  getPlayerLife();

	unsigned int getEnemyLife();

	void reducePlayerLife();

	void reduceEnemyLife();

	bool isGameStarted();

	void setBlockActions(bool flag);

	bool areActionsBlocked();

	void setDamageAreasCords(std::vector<std::vector<std::pair<float, float>>> coords);

	void startGame();

	bool checkCollision(Player& player);

	void handlePlayerMovement(sf::Event& event, Player& player, sf::RenderWindow& window, bool wantSprint);

	/* starts an internal thread */
	void waitRound();
};