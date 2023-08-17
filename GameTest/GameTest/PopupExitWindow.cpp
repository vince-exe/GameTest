#include "PopupExitWindow.h"

void PopupExitWindow::init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupExitWindowValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor) {
	if (!PopupExitWindow::loadTextures()) {
		checker = PopupExitWindowValues::TEXTURE_FAIL;
		return;
	}
	backBtn.getSprite().setPosition(615.f, 550.f);
	exitBtn.getSprite().setPosition(965.f, 550.f);

	/* center the sprite */
	sf::Vector2f spriteSize(text.getSprite().getGlobalBounds().width, text.getSprite().getGlobalBounds().height);
	sf::Vector2f windowSize(windowPtr->getSize().x, windowPtr->getSize().y);
	sf::Vector2f spritePosition((windowSize.x - spriteSize.x) / 2, (windowSize.y - spriteSize.y) / 2);
	spritePosition.y -= 100.f;
	text.getSprite().setPosition(spritePosition);

	sf::Event event;
	while (windowPtr->isOpen()) {
		while (windowPtr->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				windowPtr->close();
			}
			/* change the mouse cursor from handle to point if it is on a button */
			if (event.type == sf::Event::MouseMoved) {
				sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

				if (backBtn.isInside(position) || exitBtn.isInside(position)) {
					windowPtr->setMouseCursor(pointCursor);
				}
				else {
					windowPtr->setMouseCursor(defCursor);
				}
			}
			/* check if the user pressed one of the buttons */
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

				if (exitBtn.isInside(position)) {
					windowPtr->close();
					checker = PopupExitWindowValues::EXIT;
				}
				else if (backBtn.isInside(position)) {
					checker = PopupExitWindowValues::BACK;
					return;
				}
			}
		}
		windowPtr->clear();

		windowPtr->draw(background);
		windowPtr->draw(backBtn);
		windowPtr->draw(exitBtn);
		windowPtr->draw(text);

		windowPtr->display();
	}
}

bool PopupExitWindow::loadTextures() {
	return backBtn.loadTexture("assets/Back_Button.png") && exitBtn.loadTexture("assets/exitBtn.png") &&
		   text.loadTexture("assets/img.png");
}
