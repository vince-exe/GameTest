#include "MenuConfirmationExit.h"

void MenuConfirmationExit::init(std::shared_ptr<sf::RenderWindow> windowPtr, PopupReturnValues& checker) {
	this->windowPtr = windowPtr;

	setTextures();
	setSprite();

	sf::Event event;
	bool exitRequested = false;

	while (windowPtr->isOpen() && !exitRequested) {
		while (windowPtr->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				windowPtr->close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				checker = PopupReturnValues::BACK;
				exitRequested = true;
			}
			handleButtonClicks(checker, event, exitRequested);
		}
		draw();
	}
}

void MenuConfirmationExit::setTextures() {
	backBtn.setTexture(MainMenuTextureManager::cancelText);
	exitBtn.setTexture(MainMenuTextureManager::doneText);
	text.setTexture(MainMenuTextureManager::confirmationExitText);
}

void MenuConfirmationExit::setSprite() {
	text.getSprite().setPosition((windowPtr->getSize().x - text.getTexture().getSize().x) / 2, 230);
	backBtn.getSprite().setPosition((windowPtr->getSize().x - backBtn.getTexture().getSize().x) / 2 - 120, 390.f);
	exitBtn.getSprite().setPosition((windowPtr->getSize().x - exitBtn.getTexture().getSize().x) / 2 + 120, 390.f);
}

void MenuConfirmationExit::handleButtonClicks(PopupReturnValues& checker, sf::Event& event, bool& exitRequested) {
	sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (exitBtn.isInside(position)) {
			windowPtr->close();
			checker = PopupReturnValues::EXIT;
			exitRequested = true;
		}
		else if (backBtn.isInside(position)) {
			checker = PopupReturnValues::BACK;
			exitRequested = true;
		}
	}
}

void MenuConfirmationExit::draw() {
	windowPtr->clear();
	windowPtr->draw(backBtn);
	windowPtr->draw(exitBtn);
	windowPtr->draw(text);
	windowPtr->display();
}