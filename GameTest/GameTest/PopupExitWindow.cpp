#include "PopupExitWindow.h"

void PopupExitWindow::init(Entity& background, PopupExitWindowValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor) {
	if (!PopupExitWindow::loadTextures()) {
		checker = PopupExitWindowValues::TEXTURE_FAIL;
		return;
	}

	window.create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::Fullscreen);
	window.setFramerateLimit(60);
	
	backBtn.getSprite().setPosition(615.f, 550.f);
	exitBtn.getSprite().setPosition(965.f, 550.f);

	/* center the sprite */
	sf::Vector2f spriteSize(text.getSprite().getGlobalBounds().width, text.getSprite().getGlobalBounds().height);
	sf::Vector2f windowSize(window.getSize().x, window.getSize().y);
	sf::Vector2f spritePosition((windowSize.x - spriteSize.x) / 2, (windowSize.y - spriteSize.y) / 2);
	spritePosition.y -= 100.f;
	text.getSprite().setPosition(spritePosition);

	sf::Event event;
	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			/* change the mouse cursor from handle to point if it is on a button */
			if (event.type == sf::Event::MouseMoved) {
				sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				if (backBtn.isInside(position) || exitBtn.isInside(position)) {
					window.setMouseCursor(pointCursor);
				}
				else {
					window.setMouseCursor(defCursor);
				}
			}
			/* check if the user pressed one of the buttons */
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
				sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

				if (exitBtn.isInside(position)) {
					window.close();
					checker = PopupExitWindowValues::EXIT;
				}
				else if (backBtn.isInside(position)) {
					window.close();
				}
			}
		}
		window.clear();

		window.draw(background);
		window.draw(backBtn);
		window.draw(exitBtn);
		window.draw(text);

		window.display();
	}
}

bool PopupExitWindow::loadTextures() {
	return backBtn.loadTexture("assets/Back_Button.png") && exitBtn.loadTexture("assets/exitBtn.png") &&
		   text.loadTexture("assets/img.png");
}
