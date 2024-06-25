#include "Game.h"

Game::Game() {
	m_blockActions.store(false);
	m_gameStarted = false;
	m_currentRound = 0;
	m_playerLife = 3;
	m_enemyLife = 3;
	m_waitTimeRound = 3;
}

void Game::initSprites(sf::RenderWindow& window) {
	m_waitRoundText.setFont(FontManager::fredokaOne);
	m_waitRoundText.setCharacterSize(80);
	m_waitRoundText.setFillColor(sf::Color(255, 255, 255));

	m_waitRoundText.setPosition((window.getSize().x / 2.f) - m_waitRoundText.getGlobalBounds().width, (window.getSize().y / 2.f) - m_waitRoundText.getGlobalBounds().height - 100.f);
}

void Game::drawDamageAreasShapes(sf::RenderWindow& window) {
	for (sf::CircleShape& shape : m_damageAreasVector) {
		window.draw(shape);
	}
}

void Game::drawWaitRoundText(sf::RenderWindow& window) {
	window.draw(m_waitRoundText);
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

void Game::waitRound() {
	setBlockActions(true);
	std::thread t([this] {
		int i = 0;
		using namespace std::chrono_literals;

		while (i < m_waitTimeRound) {
			m_waitRoundText.setString(std::to_string(m_waitTimeRound - i));
			std::this_thread::sleep_for(1s);
			i++;
		}

		setBlockActions(false);
	});
	t.detach();
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