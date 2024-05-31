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
	backBtn.setTexture(MainMenuTextureManager::backText);
	exitBtn.setTexture(MainMenuTextureManager::exitText);
	text.setTexture(MainMenuTextureManager::confirmationExitText);
}

void MenuConfirmationExit::setSprite() {
	backBtn.getSprite().setPosition(615.f, 550.f);
	exitBtn.getSprite().setPosition(965.f, 550.f);

	/* center the text */
	sf::Vector2f spriteSize(text.getSprite().getGlobalBounds().width, text.getSprite().getGlobalBounds().height);
	sf::Vector2f windowSize(windowPtr->getSize().x, windowPtr->getSize().y);
	sf::Vector2f spritePosition((windowSize.x - spriteSize.x) / 2, (windowSize.y - spriteSize.y) / 2);
	spritePosition.y -= 100.f;
	text.getSprite().setPosition(spritePosition);
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