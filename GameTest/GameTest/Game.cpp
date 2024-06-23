#include "Game.h"

Game::Game() {
	m_blockActions = false;
	m_gameStarted = false;
	m_currentRound = 0;
}

bool Game::isGameStarted() {
	return m_gameStarted;
}

void Game::setGameWindow(std::shared_ptr<sf::RenderWindow> window) {
	m_gameWindow = window;
}

void Game::blockActions(bool flag) {
	m_blockActions = flag;
}

void Game::setDamageAreasCords(std::vector<std::vector<std::pair<float, float>>> coords) {
	m_damageAreasCoordinates = coords;
}

void Game::startGame() {
	float posX, posY;
	
	for (int i = 0; i < 6; i++) {
		m_damageAreasVector.push_back(sf::CircleShape());

		m_damageAreasVector.at(i).setRadius(60.f);
		m_damageAreasVector.at(i).setOutlineThickness(8.f);
		m_damageAreasVector.at(i).setFillColor(sf::Color(120, 36, 14));
		m_damageAreasVector.at(i).setOutlineColor(sf::Color(82, 20, 5));

		posX = m_damageAreasCoordinates.at(m_currentRound).at(i).first;
		posY = m_damageAreasCoordinates.at(m_currentRound).at(i).second;
		
		m_damageAreasVector.at(i).setPosition(posX, posY);
	}
	m_gameStarted = true;
}

void Game::drawDamageAreas() {
	for (sf::CircleShape& shape : m_damageAreasVector) {
		m_gameWindow->draw(shape);
	}
}
