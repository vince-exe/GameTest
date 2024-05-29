#include "GameSettingsMenu.h"

void GameSettingsMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr) {
	this->windowPtr = windowPtr;
	
	setTextures();
	setSprites();

	sf::Event event;
	/* we can't use the close() of the windowPtr because if we do this, even the MainGameWindow is closed */
	bool exitWindow = false;
	while (!exitWindow) {
		while (this->windowPtr->pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				exitWindow = true;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

				if (backText.isInside(position)) {
					exitWindow = true;
				}
			}
		}
		draw();
	}
}

void GameSettingsMenu::setTextures() {
	backText.setTexture(MainGameTextureManager::backGameOptions);
}

void GameSettingsMenu::setSprites() {
	backText.getSprite().setPosition(20.f, windowPtr->getSize().y - backText.getSprite().getTextureRect().getSize().y - 40);
}

void GameSettingsMenu::draw() {
	windowPtr->clear();

	windowPtr->draw(backText);

	windowPtr->display();
}
