#include "GameSettingsMenu.h"

bool GameSettingsMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, std::atomic<bool>* closeFlag) {
	this->closeFlag = closeFlag;
	this->windowPtr = windowPtr;

	setTextures();
	setSprites();
	
	sf::Event event;
	/* we can't use the close() of the windowPtr because if we do this, even the MainGameWindow is closed */
	this->exitWindow = false;
	while (!this->exitWindow && !this->closeFlag->load()) {
		while (windowPtr->pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				return false;
			}
			handleMouseButtonPressed(event);
		}
		draw();
	}

	return (this->closeFlag->load());
}

void GameSettingsMenu::setTextures() {
	backText.setTexture(MainMenuTextureManager::cancelText);
}

void GameSettingsMenu::setSprites() {
	backText.getSprite().setPosition(20.f, windowPtr->getSize().y - backText.getSprite().getGlobalBounds().height - 40);
}

void GameSettingsMenu::draw() {
	windowPtr->clear();

	windowPtr->draw(backText);

	windowPtr->display();
}

void GameSettingsMenu::handleMouseButtonPressed(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

		if (backText.isInside(position)) {
			this->exitWindow = true;
		}
	}
}
