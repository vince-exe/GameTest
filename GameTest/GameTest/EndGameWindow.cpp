#include "EndGameWindow.h" 

void EndGameWindow::initSprites(sf::RenderWindow& window, Game& game) {
	m_resultsText.setFont(FontManager::fredokaOne);
	m_resultsText.setCharacterSize(80);
	m_resultsText.setPosition(sf::Vector2f(450, 350));

	if (game.getGameResults() == Game::GameResults::WON) {
		m_resultsText.setFillColor(sf::Color(31, 110, 2));
		m_resultsText.setString("Hai Vinto!");
	}
	else {
		m_resultsText.setFillColor(sf::Color(110, 6, 2));
		m_resultsText.setString("Hai Perso!");
	}
}

void EndGameWindow::draw(sf::RenderWindow& window) {
	window.clear();

	window.draw(m_resultsText);

	window.display();
}

void EndGameWindow::init(sf::RenderWindow& window, Game& game) {
	m_closeWindow = false;

	initSprites(window, game);

	sf::Event event;
	while (!m_closeWindow) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_closeWindow = true;
			}
		}

		draw(window);
	}
}
