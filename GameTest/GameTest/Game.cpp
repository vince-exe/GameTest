#include "Game.h"

Game::Game() {
	m_blockActions = false;
	m_gameStarted = false;
	m_currentRound = 0;
	m_playerLife = 3;
	m_enemyLife = 3;
}

void Game::drawDamageAreasShapes(sf::RenderWindow& window) {
	for (sf::CircleShape& shape : m_damageAreasVector) {
		window.draw(shape);
	}
}

sf::Vector2f Game::getStartPlayerPosition() {
	return m_startPlayerPosition;
}

void Game::setPlayerStartPosition(sf::Vector2f vec) {
	m_startPlayerPosition = vec;
}

bool Game::isGameStarted() {
	return m_gameStarted;
}

void Game::blockActions(bool flag) {
	m_blockActions = flag;
}

unsigned int Game::getPlayerLife() {
	return m_playerLife;
}

unsigned int Game::getEnemyLife() {
	return m_enemyLife;
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

void Game::reducePlayerLife() {
	m_playerLife--;
}

void Game::reduceEnemyLife() {
	m_enemyLife--;
}

bool Game::checkCollision(Player& player) {
	for (sf::CircleShape& shape : m_damageAreasVector) {
		if (shape.getGlobalBounds().intersects(player.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}
