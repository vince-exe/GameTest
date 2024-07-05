#include "GameSettingsMenu.h"

bool GameSettingsMenu::init(sf::RenderWindow& window, std::atomic<bool>* closeFlag) {
	m_closeFlag = closeFlag;
	m_Window = &window;

	setTextures();
	setSprites();
	
	sf::Event event;
	/* we can't use the close() of the m_Window because if we do this, even the MainGameWindow is closed */
	m_exitWindow = false;
	while (!m_exitWindow && !m_closeFlag->load()) {
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				return false;
			}
			handleMouseButtonPressed(event);
		}
		draw();
	}

	return (m_closeFlag->load());
}

void GameSettingsMenu::setTextures() {
	m_backText.setTexture(MainMenuTextureManager::cancelText);
}

void GameSettingsMenu::setSprites() {
	m_backText.getSprite().setPosition(20.f, m_Window->getSize().y - m_backText.getSprite().getGlobalBounds().height - 40);
}

void GameSettingsMenu::draw() {
	m_Window->clear();

	m_Window->draw(m_backText);

	m_Window->display();
}

void GameSettingsMenu::handleMouseButtonPressed(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

		if (m_backText.isInside(position)) {
			m_exitWindow = true;
		}
	}
}
