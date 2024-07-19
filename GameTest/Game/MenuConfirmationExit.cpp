#include "MenuConfirmationExit.h"

void MenuConfirmationExit::init(sf::RenderWindow& window, TextureManager& textureManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker) {
	m_Window = &window;

	setTextures(textureManager);
	setSprite();

	sf::Event event;
	bool exitRequested = false;

	while (m_Window->isOpen() && !exitRequested) {
		while (m_Window->pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				m_Window->close();
			}
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
				checker = SkyfallUtils::WindowsReturnValues::BACK;
				exitRequested = true;
			}
			handleButtonClicks(checker, event, exitRequested, audioManager);
		}
		draw();
	}
}

void MenuConfirmationExit::setTextures(TextureManager& textureManager) {
	m_backBtn.setTexture(textureManager.getCancelBtn());
	m_exitBtn.setTexture(textureManager.getDoneBtn());
	m_Text.setTexture(textureManager.getTextImage(6));
}

void MenuConfirmationExit::setSprite() {
	m_Text.getSprite().setPosition((m_Window->getSize().x - m_Text.getTexture().getSize().x) / 2, 230);
	m_backBtn.getSprite().setPosition((m_Window->getSize().x - m_backBtn.getTexture().getSize().x) / 2 - 120, 390.f);
	m_exitBtn.getSprite().setPosition((m_Window->getSize().x - m_exitBtn.getTexture().getSize().x) / 2 + 120, 390.f);
}

void MenuConfirmationExit::handleButtonClicks(SkyfallUtils::WindowsReturnValues& checker, sf::Event& event, bool& exitRequested, AudioManager& audioManager) {
	sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		if (m_exitBtn.isInside(position)) {
			audioManager.getButtonClickSound().play();
			m_Window->close();
			checker = SkyfallUtils::WindowsReturnValues::EXIT;
			exitRequested = true;
		}
		else if (m_backBtn.isInside(position)) {
			audioManager.getButtonClickSound().play();
			checker = SkyfallUtils::WindowsReturnValues::BACK;
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