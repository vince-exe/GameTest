#include "GameSettingsMenu.h"

void GameSettingsMenu::setTextures() {
	m_backText.setTexture(g_tSingleton.getCancelBtn());
}

void GameSettingsMenu::setSprites(sf::RenderWindow& window) {
	m_backText.getSprite().setPosition(20.f, window.getSize().y - m_backText.getSprite().getGlobalBounds().height - 40);
}

void GameSettingsMenu::draw(sf::RenderWindow& window) {
	window.clear();

	window.draw(m_backText);

	window.display();
}

void GameSettingsMenu::handleMouseButtonPressed(sf::Event& event, sf::RenderWindow& window, bool& inGameSettings) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		if (m_backText.isInside(position)) {
			inGameSettings = false;
		}
	}
}
