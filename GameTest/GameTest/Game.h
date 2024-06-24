#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>

#include "Player.h"

class Game {
private:
	std::vector<std::vector<std::pair<float, float>>> m_damageAreasCoordinates;
	std::vector<sf::CircleShape> m_damageAreasVector;

	bool m_blockActions;
	bool m_gameStarted;
	unsigned int m_currentRound;
	unsigned int m_playerLife, m_enemyLife;
	sf::Vector2f m_startPlayerPosition;

public:
	Game();

	void drawDamageAreasShapes(sf::RenderWindow& window);

	void setPlayerStartPosition(sf::Vector2f vec);
	
	sf::Vector2f getStartPlayerPosition();

	unsigned int  getPlayerLife();

	unsigned int getEnemyLife();

	void reducePlayerLife();

	void reduceEnemyLife();

	bool isGameStarted();

	void blockActions(bool flag);

	void setDamageAreasCords(std::vector<std::vector<std::pair<float, float>>> coords);

	void startGame();

	bool checkCollision(Player& player);
};