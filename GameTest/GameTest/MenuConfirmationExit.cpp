#include "MenuConfirmationExit.h"

void MenuConfirmationExit::init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupReturnValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor) {
	this->windowPtr = windowPtr;
	windowPtr->setMouseCursor(defCursor);

	setTextures();
	setSprite();

	sf::Event event;
	bool exitRequested = false;

	while (windowPtr->isOpen() && !exitRequested) {
		while (windowPtr->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				windowPtr->close();
			}
			handleMouseCursor(pointCursor, defCursor);

			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				handleButtonClicks(checker, exitRequested);
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				checker = PopupReturnValues::BACK;
				exitRequested = true;
			}
		}
		renderWindow(background);
	}
}

void MenuConfirmationExit::setTextures() {
	backBtn.setTexture(MainMenuTextureManager::backBtn);
	exitBtn.setTexture(MainMenuTextureManager::exitBtn);
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

void MenuConfirmationExit::handleMouseCursor(sf::Cursor& pointCursor, sf::Cursor& defCursor) {
	sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

	if (backBtn.isInside(position) || exitBtn.isInside(position)) {
		windowPtr->setMouseCursor(pointCursor);
	}
	else {
		windowPtr->setMouseCursor(defCursor);
	}
}

void MenuConfirmationExit::handleButtonClicks(PopupReturnValues& checker, bool& exitRequested) {
	sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

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

void MenuConfirmationExit::renderWindow(Entity& background) {
	windowPtr->clear();
	windowPtr->draw(background);
	windowPtr->draw(backBtn);
	windowPtr->draw(exitBtn);
	windowPtr->draw(text);
	windowPtr->display();
}