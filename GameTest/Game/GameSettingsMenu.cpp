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
    m_musicText.setFont(g_fSingleton.getFredokaOne());
    m_musicText.setCharacterSize(50);
    m_musicText.setString("Music");
    m_musicText.setPosition(700.f, 35.f);
    m_musicText.setFillColor(sf::Color(97, 24, 24));

    m_musicSlider.setSize(sf::Vector2f(500, 10));
    m_musicSlider.setFillColor(sf::Color(163, 163, 163));
    m_musicSlider.setPosition((m_musicText.getPosition().x / 2) + (m_musicSlider.getGlobalBounds().width / 2 - m_musicText.getGlobalBounds().width / 2), m_musicText.getPosition().y + m_musicText.getGlobalBounds().height + 45);

    for (int i = 0; i < 10; ++i) {
        m_musicCheckPoints[i].setSize(sf::Vector2f(15, 50));
        m_musicCheckPoints[i].setFillColor(m_defCheckpointColor);
        m_musicCheckPoints[i].setPosition(sf::Vector2f(m_musicSlider.getPosition().x + i * (m_musicSlider.getSize().x / 9), m_musicSlider.getPosition().y - 21));
    }

    m_soundEffectsText.setFont(g_fSingleton.getFredokaOne());
    m_soundEffectsText.setCharacterSize(50);
    m_soundEffectsText.setString("Sound-Effects");
    m_soundEffectsText.setPosition(670.f - (m_musicText.getGlobalBounds().width / 2), m_musicSlider.getPosition().y + 46);
    m_soundEffectsText.setFillColor(sf::Color(97, 24, 24));

    m_soundEffectsSlider.setSize(sf::Vector2f(500, 10));
    m_soundEffectsSlider.setFillColor(sf::Color(163, 163, 163));
    m_soundEffectsSlider.setPosition((m_musicText.getPosition().x / 2) + (m_musicSlider.getGlobalBounds().width / 2 - m_musicText.getGlobalBounds().width / 2), m_soundEffectsText.getPosition().y + m_soundEffectsText.getGlobalBounds().height + 45);

    for (int i = 0; i < 10; ++i) {
        m_soundEffectsCheckpoints[i].setSize(sf::Vector2f(15, 50));
        m_soundEffectsCheckpoints[i].setFillColor(m_defCheckpointColor);
        m_soundEffectsCheckpoints[i].setPosition(sf::Vector2f(m_soundEffectsSlider.getPosition().x + i * (m_soundEffectsSlider.getSize().x / 9), m_soundEffectsSlider.getPosition().y - 21));
    }

    /* set the current level of sound effects */
    int soundVolume = g_aSingleton.getSoundEffectsVolume();
    if (soundVolume == 0) {
        m_soundEffectsCheckpoints[0].setFillColor(m_selectedCheckpointColor);
        m_oldSoundVolumeIndex = 0;
    }
    else {
        m_soundEffectsCheckpoints[soundVolume / 10].setFillColor(m_selectedCheckpointColor);
        m_oldSoundVolumeIndex = soundVolume / 10;
    }
    /* set the current level of music */
    int musicVolume = g_aSingleton.getBackgroundMusic().getVolume();
    if (musicVolume == 0) {
        m_musicCheckPoints[0].setFillColor(m_selectedCheckpointColor);
        m_oldMusicVolumeIndex = 0;
    }
    else {
        m_musicCheckPoints[musicVolume / 5].setFillColor(m_selectedCheckpointColor);
        m_oldMusicVolumeIndex = musicVolume / 5;
    }
}

void GameSettingsMenu::draw(sf::RenderWindow& window) {
	window.clear();

	window.draw(m_backText);
    window.draw(m_debugModeText);
    window.draw(m_debugModeResult);
    window.draw(m_musicSlider);
    window.draw(m_musicText);
    window.draw(m_soundEffectsSlider);
    window.draw(m_soundEffectsText);

    for (int i = 0; i < 10; i++) {
        window.draw(m_musicCheckPoints[i]);
    }
    for (int i = 0; i < 10; i++) {
        window.draw(m_soundEffectsCheckpoints[i]);
    }

	window.display();
}

void GameSettingsMenu::handleMouseButtonPressed(sf::Event& event, sf::RenderWindow& window, bool& inGameSettings, Player& player) {
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        setMusicLevel(window, position);
        setSoundLevel(window, position);

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

void GameSettingsMenu::setMusicLevel(sf::RenderWindow& window, sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    for (int i = 0; i < 10; i++) {
        if (m_musicCheckPoints[i].getGlobalBounds().contains(position)) {
            m_musicCheckPoints[m_oldMusicVolumeIndex].setFillColor(m_defCheckpointColor);
            m_oldMusicVolumeIndex = i;
            m_musicCheckPoints[i].setFillColor(m_selectedCheckpointColor);
            
            g_aSingleton.getBattleMusic().setVolume(volumeLevel);
            g_sSingleton.setInt_(SkyfallUtils::Settings::MUSIC_VOLUME, volumeLevel);
        }
        volumeLevel += 5;
    }
}

void GameSettingsMenu::setSoundLevel(sf::RenderWindow& window, sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition();
    for (int i = 0; i < 10; i++) {
        if (m_soundEffectsCheckpoints[i].getGlobalBounds().contains(position)) {
            m_soundEffectsCheckpoints[m_oldSoundVolumeIndex].setFillColor(m_defCheckpointColor);
            m_oldSoundVolumeIndex = i;
            m_soundEffectsCheckpoints[i].setFillColor(m_selectedCheckpointColor);

            g_aSingleton.setSoundEffectsVolume(volumeLevel);
            g_aSingleton.getButtonClickSound().play();
            g_sSingleton.setInt_(SkyfallUtils::Settings::SOUND_EFFECTS_VOLUME, volumeLevel);
        }
        volumeLevel += 10;
    }
}