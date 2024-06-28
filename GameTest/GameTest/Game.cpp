#include "Game.h"

Game::Game() {
	m_blockActions.store(false);
	m_gameState = GameStates::NOT_RUNNING;
	m_currentRound = 0;
	m_playerLife = 3;
	m_enemyLife = 3;
	m_waitTimeRound = 3;
}

sf::Vector2f Game::getStartPlayerPosition() {
	return m_startPlayerPosition;
}

void Game::setPlayerStartPosition(sf::Vector2f vec) {
	m_startPlayerPosition = vec;
}

void Game::setBlockActions(bool flag) {
	m_blockActions.store(flag);
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

bool Game::areActionsBlocked() {
	return m_blockActions.load();
}

void Game::waitRound(sf::Text& text) {
	setBlockActions(true);
	std::thread t([this, &text] {
		int i = 0;
		using namespace std::chrono_literals;

		while (i < m_waitTimeRound) {
			text.setString(std::to_string(m_waitTimeRound - i));
			std::this_thread::sleep_for(1s);
			i++;
		}

		setBlockActions(false);
	});
	t.detach();
}

void Game::handleNewRound(GameEntities entity) {
	switch (entity) {
	case GameEntities::PLAYER:
		m_playerLife--;
		break;

	case GameEntities::ENEMY:
		m_enemyLife--;
		break;
	}
	m_currentRound++;
	if (m_currentRound >= 3) {
		m_gameState = GameStates::END;
	}
}

void Game::startGame(std::vector<std::vector<sf::CircleShape>>& finalVector) {	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 6; j++) {
			finalVector.at(i).at(j).setPosition(m_damageAreasCoordinates.at(i).at(j).first, m_damageAreasCoordinates.at(i).at(j).second);
		}
	}
	m_gameState = GameStates::RUNNING;
}

bool Game::checkCollision(std::vector<sf::CircleShape> vec, Player& player) {
	for (sf::CircleShape& shape : vec) {
		if (shape.getGlobalBounds().intersects(player.getGlobalBounds())) {
			return true;
		}
	}
	return false;
}

void Game::handlePlayerMovement(sf::Event& event, Player& player, sf::RenderWindow& window, bool wantSprint) {
	if (!m_blockActions.load()) {
		const sf::Vector2i mousePosition{ sf::Mouse::getPosition(window) };
		const sf::Vector2f mouseCoord{ window.mapPixelToCoords(mousePosition) };

		if (wantSprint) {
			if (player.canSprint()) {
				player.startSprint(true);
				player.setTarget(mouseCoord);
			}
		}
		else {
			player.setTarget(mouseCoord);
		}
	}
}

Game::GameResults Game::getGameResults() {
	return (m_playerLife > m_enemyLife) ? GameResults::WON : GameResults::LOST;
}

Game::GameStates Game::getGameState() {
	return m_gameState;
}

unsigned int Game::getCurrentRound() {
	return m_currentRound;
}