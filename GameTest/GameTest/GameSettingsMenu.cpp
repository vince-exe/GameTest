#include "GameSettingsMenu.h"

void GameSettingsMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr) {
	this->windowPtr = windowPtr;
	
	setTextures();
	setSprites();

	sf::Event event;
	/* we can't use the close() of the windowPtr because if we do this, even the MainGameWindow is closed */
	this->exitWindow = false;
	while (!this->exitWindow) {
		while (this->windowPtr->pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				this->exitWindow = true;
			}
			handleMouseMovement(event);
			handleMouseButtonPressed(event);
		}
		draw();
	}
}

void GameSettingsMenu::setTextures() {
	backText.setTexture(MainGameTextureManager::backGameOptions);
}

void GameSettingsMenu::setSprites() {
	backText.getSprite().setPosition(20.f, windowPtr->getSize().y - backText.getSprite().getGlobalBounds().height - 40);

	backTextUnderLine.setSize(sf::Vector2f(backText.getSprite().getGlobalBounds().width, 3));
	backTextUnderLine.setFillColor(sf::Color(181, 181, 181));
	backTextUnderLine.setRotation(0);
	backTextUnderLine.setPosition(20, backText.getSprite().getPosition().y + backText.getSprite().getGlobalBounds().height + 3);
}

void GameSettingsMenu::draw() {
	windowPtr->clear();

	windowPtr->draw(backText);
	windowPtr->draw(backTextUnderLine);

	windowPtr->display();
}

void GameSettingsMenu::handleMouseMovement(sf::Event& event) {
	if (event.type == sf::Event::MouseMoved) {
		sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

		if (backText.isInside(position)) {
			backTextUnderLine.setFillColor(sf::Color(70, 136, 227));
		}
		else {
			backTextUnderLine.setFillColor(sf::Color(237, 237, 237));
		}
	}
}

void GameSettingsMenu::handleMouseButtonPressed(sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

		if (backText.isInside(position)) {
			this->exitWindow = true;
		}
	}
}
