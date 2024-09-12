#include "Game.h"

Game::Game() {
	m_blockActions.store(false);
	m_gameState = GameStates::NOT_RUNNING;
	m_currentRound = 0;
	m_playerLife = 3;
	m_enemyLife = 3;
	m_waitTimeRound = 3;
	m_gameMaxTime = 3;
	m_enemyQuit = false;
}

const sf::Vector2f& Game::getStartPlayerPosition() const {
	return m_startPlayerPosition;
}

const sf::Vector2f& Game::getEnemyStartPosition() const {
	return m_startEnemyPosition;
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

void Game::waitRound(sf::Text& text, Sound& soundToPlay) {
	setBlockActions(true);
	std::thread t([this, &text, &soundToPlay] {
		int i = 0;
		using namespace std::chrono_literals;
		soundToPlay.play();
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
		m_currentRound = 0;
	}
	if (m_playerLife <= 0 || m_enemyLife <= 0) {
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

void Game::startTimer(sf::Text& text) {
	using namespace std::chrono_literals;

	std::thread t([this, &text] {
		unsigned int seconds = 59;
		sf::Color warningColor(186, 141, 19);

		while (m_gameState == GameStates::RUNNING) {
			if (seconds < 10) {
				text.setString(std::to_string(m_gameMaxTime) + " : 0" + std::to_string(seconds));
			}
			else {
				text.setString(std::to_string(m_gameMaxTime) + " : " + std::to_string(seconds));
			}
			
			if (m_gameMaxTime == 0) {
				text.setFillColor(warningColor);
			}
			if (seconds == 0 && m_gameMaxTime == 0) {
				m_gameState = GameStates::END;
				return;
			}
			else if (seconds == 0) {
				m_gameMaxTime--;
				seconds = 59;
			}
			else {
				seconds--;
			}
			std::this_thread::sleep_for(1s);
		}
	});
	t.detach();
}

void Game::handleEnemyQuit() {
	m_playerLife = 1000;
	m_enemyQuit = true;
	m_gameState = GameStates::END;
}

bool Game::hasEnemyQuit() {
	return m_enemyQuit;
}

Game::GameResults Game::getGameResults() {
	if (m_playerLife > m_enemyLife) {
		return GameResults::WON;
	}
	else if (m_playerLife < m_enemyLife) {
		return GameResults::LOST;
	}
	return GameResults::DRAW;
}

void Game::setPlayerPosition(const sf::Vector2f& vec1) {
	m_startPlayerPosition = vec1;
}

void Game::setEnemyPosition(const sf::Vector2f& vec2) {
	m_startEnemyPosition = vec2;
}

Game::GameStates Game::getGameState() {
	return m_gameState;
}

unsigned int Game::getCurrentRound() {
	return m_currentRound;
}