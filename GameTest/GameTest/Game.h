#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <utility>

class Game {
private:
	std::vector<std::vector<std::pair<float, float>>> m_damageAreasCoordinates;
	std::vector<sf::CircleShape> m_damageAreasVector;
	std::shared_ptr<sf::RenderWindow> m_gameWindow;

	bool m_blockActions;
	bool m_gameStarted;
	unsigned int m_currentRound;

public:
	Game();

	bool isGameStarted();

	void setGameWindow(std::shared_ptr<sf::RenderWindow> window);

	void blockActions(bool flag);

	void setDamageAreasCords(std::vector<std::vector<std::pair<float, float>>> coords);

	void startGame();

	void drawDamageAreas();
};