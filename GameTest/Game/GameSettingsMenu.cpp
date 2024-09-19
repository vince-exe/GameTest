#include "GameSettingsMenu.h"

void GameSettingsMenu::setTextures() {
	m_backText.setTexture(g_tSingleton.getCancelBtn());
}

void GameSettingsMenu::setSprites(sf::RenderWindow& window) {
	m_backText.getSprite().setPosition(20.f, window.getSize().y - m_backText.getSprite().getGlobalBounds().height - 40);

    m_debugModeText.setFont(g_fSingleton.getFredokaOne());
    m_debugModeText.setCharacterSize(40);
    m_debugModeText.setString("Debug-Mode: ");
    m_debugModeText.setPosition(35.f, 35.f);
    m_debugModeText.setFillColor(sf::Color(199, 199, 199));

    m_debugModeResult.setFont(g_fSingleton.getFredokaOne());
    m_debugModeResult.setCharacterSize(50);
    m_debugModeResult.setString(g_sSingleton.getValue(SkyfallUtils::Settings::DEBUG_MODE).GetString());
    if (std::strcmp(g_sSingleton.getValue(SkyfallUtils::Settings::DEBUG_MODE).GetString(), "ON") == 0) {

        m_debugModeResult.setFillColor(sf::Color(43, 97, 24));
    }
    else {
        m_debugModeResult.setFillColor(sf::Color(97, 24, 24));
    }
    m_debugModeResult.setPosition(m_debugModeText.getPosition().x + m_debugModeText.getGlobalBounds().width + 15, m_debugModeText.getPosition().y - 5.f);
}

void GameSettingsMenu::draw(sf::RenderWindow& window) {
	window.clear();

	window.draw(m_backText);
    window.draw(m_debugModeText);
    window.draw(m_debugModeResult);

	window.display();
}

void GameSettingsMenu::handleMouseButtonPressed(sf::Event& event, sf::RenderWindow& window, bool& inGameSettings, Player& player) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		if (m_backText.isInside(position)) {
			inGameSettings = false;
		}
        
        if (m_debugModeResult.getGlobalBounds().contains(position)) {
            g_aSingleton.getButtonClickSound().play();
            player.setDebugMode(!player.getDebugMode());

            if (m_debugModeResult.getString().toAnsiString() == "OFF") {
                g_sSingleton.setString_(SkyfallUtils::Settings::DEBUG_MODE, "ON");
                m_debugModeResult.setFillColor(sf::Color(43, 97, 24));
                m_debugModeResult.setString("ON");
            }
            else {
                g_sSingleton.setString_(SkyfallUtils::Settings::DEBUG_MODE, "OFF");
                m_debugModeResult.setFillColor(sf::Color(97, 24, 24));
                m_debugModeResult.setString("OFF");
            }
        }
	}
}

void GameSettingsMenu::init(sf::RenderWindow& window) {
    setTextures();
    setSprites(window);
}