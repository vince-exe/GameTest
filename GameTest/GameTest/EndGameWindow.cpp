#include "EndGameWindow.h" 

void EndGameWindow::initTextures(TextureManager& textureManager) {
	m_doneButton.setTexture(textureManager.getDoneBtn());
}

void EndGameWindow::initSprites() {
	m_resultsText.setFont(FontManager::fredokaOne);
	m_resultsText.setCharacterSize(70);
	m_resultsText.setPosition(sf::Vector2f(450, 350));

	m_enemyQuitText.setFont(FontManager::fredokaOne);
	m_enemyQuitText.setCharacterSize(27);
	m_enemyQuitText.setFillColor(sf::Color::White);
	m_enemyQuitText.setString("( L'avversario ha abbandonato la partita )");
	m_enemyQuitText.setPosition(sf::Vector2f(m_resultsText.getPosition().x - 107.f, m_resultsText.getPosition().y + m_enemyQuitText.getGlobalBounds().height + 66.f));

	if (m_Game->getGameResults() == Game::GameResults::WON) {
		m_resultsText.setFillColor(sf::Color(31, 110, 2));
		m_resultsText.setString("Hai Vinto!");
	}
	else if(m_Game->getGameResults() == Game::GameResults::LOST) {
		m_resultsText.setFillColor(sf::Color(110, 6, 2));
		m_resultsText.setString("Hai Perso!");
	}
	else {	
		m_resultsText.setFillColor(sf::Color::White);
		m_resultsText.setString("Pareggio!");
	}

	m_gameText.setFont(FontManager::fredokaOne);
	m_gameText.setCharacterSize(50);
	m_gameText.setFillColor(sf::Color(110, 6, 2));
	m_gameText.setPosition(sf::Vector2f(20.f, m_Window->getSize().y - m_gameText.getGlobalBounds().height - 72.f));
	m_gameText.setString("Skyfall Show|Down");

	m_doneButton.getSprite().setPosition(sf::Vector2f(m_Window->getSize().x - m_doneButton.getSprite().getGlobalBounds().width - 20.f, m_Window->getSize().y - m_doneButton.getSprite().getGlobalBounds().height - 20.f));
}

void EndGameWindow::draw(sf::Text& playerNickText, sf::Text& vsText, sf::Text& enemyNickText) {
	m_Window->clear();

	m_Window->draw(m_resultsText);
	m_Window->draw(m_gameText);
	m_Window->draw(m_doneButton);
	m_Window->draw(playerNickText);
	m_Window->draw(vsText);
	m_Window->draw(enemyNickText);

	if (m_Game->hasEnemyQuit()) {
		m_Window->draw(m_enemyQuitText);
	}

	m_Window->display();
}

void EndGameWindow::handleMouseButtons(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

		if (m_doneButton.isInside(position)) {
			m_closeWindow = true;
		}
	}
	
}

void EndGameWindow::init(sf::RenderWindow& window, Game& game, TextureManager& textureManager, sf::Text& playerNickText, sf::Text& vsText, sf::Text& enemyNickText) {
	m_closeWindow = false;
	m_Window = &window;
	m_Game = &game;

	initTextures(textureManager);
	initSprites();

	sf::Event event;
	while (!m_closeWindow) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_closeWindow = true;
			}
		}
		handleMouseButtons(event);

		draw(playerNickText, vsText, enemyNickText);
	}
}
