#include "MenuConfirmationExit.h"

void MenuConfirmationExit::init(std::shared_ptr<sf::RenderWindow> window, UiUtils::WindowsReturnValues& checker) {
	m_Window = window;

	setTextures();
	setSprite();

	sf::Event event;
	bool exitRequested = false;

	while (m_Window->isOpen() && !exitRequested) {
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Window->close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				checker = UiUtils::WindowsReturnValues::BACK;
				exitRequested = true;
			}
			handleButtonClicks(checker, event, exitRequested);
		}
		draw();
	}
}

void MenuConfirmationExit::setTextures() {
	m_backBtn.setTexture(MainMenuTextureManager::cancelText);
	m_exitBtn.setTexture(MainMenuTextureManager::doneText);
	m_Text.setTexture(MainMenuTextureManager::confirmationExitText);
}

void MenuConfirmationExit::setSprite() {
	m_Text.getSprite().setPosition((m_Window->getSize().x - m_Text.getTexture().getSize().x) / 2, 230);
	m_backBtn.getSprite().setPosition((m_Window->getSize().x - m_backBtn.getTexture().getSize().x) / 2 - 120, 390.f);
	m_exitBtn.getSprite().setPosition((m_Window->getSize().x - m_exitBtn.getTexture().getSize().x) / 2 + 120, 390.f);
}

void MenuConfirmationExit::handleButtonClicks(UiUtils::WindowsReturnValues& checker, sf::Event& event, bool& exitRequested) {
	sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_exitBtn.isInside(position)) {
			m_Window->close();
			checker = UiUtils::WindowsReturnValues::EXIT;
			exitRequested = true;
		}
		else if (m_backBtn.isInside(position)) {
			checker = UiUtils::WindowsReturnValues::BACK;
			exitRequested = true;
		}
	}
}

void MenuConfirmationExit::draw() {
	m_Window->clear();
	m_Window->draw(m_backBtn);
	m_Window->draw(m_exitBtn);
	m_Window->draw(m_Text);
	m_Window->display();
}