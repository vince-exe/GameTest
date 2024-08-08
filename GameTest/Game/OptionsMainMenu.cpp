#include "OptionsMainMenu.h"

void OptionsMainMenu::init(sf::RenderWindow& window, SkyfallUtils::WindowsReturnValues& checker) {
    m_Window = &window;

    sf::Cursor defaultCursor;
    defaultCursor.loadFromSystem(sf::Cursor::Arrow);
    m_Window->setMouseCursor(defaultCursor);
    
    setTextures();
    initSprites();

    bool requestExit = false;
    sf::Event event;
    while (m_Window->isOpen() && !requestExit) {
        while (m_Window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_Window->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                checker = SkyfallUtils::WindowsReturnValues::BACK;
                return;
            }
            handleMouseButtons(event, requestExit, checker);
        }
        draw();
    }
}

void OptionsMainMenu::draw() {
    m_Window->clear();
    m_Window->draw(m_backBtn);
    m_Window->draw(m_musicSlider);
    m_Window->draw(m_musicText);
    m_Window->draw(m_soundEffectsText);
    m_Window->draw(m_debugModeText);
    m_Window->draw(m_debugModeResult);

    m_Window->draw(m_soundEffectsSlider);
    for (int i = 0; i < 10; i++) {
        m_Window->draw(m_musicCheckPoints[i]);
    }
    for (int i = 0; i < 10; i++) {
        m_Window->draw(m_soundEffectsCheckpoints[i]);
    }

    m_Window->display();
}

void OptionsMainMenu::handleMouseButtons(sf::Event& event, bool& requestExit, SkyfallUtils::WindowsReturnValues& checker) {
    sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        setMusicLevel(position);
        setSoundLevel(position);
        
        if (m_backBtn.isInside(position)) {
            g_aSingleton.getButtonClickSound().play();
            requestExit = true;
            checker = SkyfallUtils::WindowsReturnValues::BACK;
            return;
        }
        if (m_debugModeResult.getGlobalBounds().contains(position)) {
            g_aSingleton.getButtonClickSound().play();

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

void OptionsMainMenu::setMusicLevel(sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_Window);
    for (int i = 0; i < 10; i++) {
        if (m_musicCheckPoints[i].getGlobalBounds().contains(position)) {
            m_musicCheckPoints[m_oldMusicVolumeIndex].setFillColor(m_defCheckpointColor);
            m_oldMusicVolumeIndex = i;
            m_musicCheckPoints[i].setFillColor(m_selectedCheckpointColor);

            g_aSingleton.getBackgroundMusic().setVolume(volumeLevel);
            g_sSingleton.setInt_(SkyfallUtils::Settings::MUSIC_VOLUME, volumeLevel);
        } 
        volumeLevel += 5;
    }
}

void OptionsMainMenu::setSoundLevel(sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_Window);
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

void OptionsMainMenu::setTextures() {
    m_backBtn.setTexture(g_tSingleton.getCancelBtn());
}

void OptionsMainMenu::initSprites() {
    m_musicText.setFont(g_fSingleton.getFredokaOne());
    m_musicText.setCharacterSize(50);
    m_musicText.setString("Music");
    m_musicText.setPosition(m_Window->getSize().x / 2 - m_musicText.getGlobalBounds().width + 70.f, 10.f);
    m_musicText.setFillColor(sf::Color(97, 24, 24));

    m_musicSlider.setSize(sf::Vector2f(500, 10));
    m_musicSlider.setFillColor(sf::Color(163, 163, 163));
    m_musicSlider.setPosition((m_Window->getSize().x - m_musicSlider.getSize().x) / 2, m_musicText.getPosition().y + m_musicText.getGlobalBounds().height + 45);

    for (int i = 0; i < 10; ++i) {
        m_musicCheckPoints[i].setSize(sf::Vector2f(15, 50));
        m_musicCheckPoints[i].setFillColor(m_defCheckpointColor);
        m_musicCheckPoints[i].setPosition(sf::Vector2f(m_musicSlider.getPosition().x + i * (m_musicSlider.getSize().x / 9), m_musicSlider.getPosition().y - 21));
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

    m_soundEffectsText.setFont(g_fSingleton.getFredokaOne());
    m_soundEffectsText.setCharacterSize(50);
    m_soundEffectsText.setString("Sound-Effects");
    m_soundEffectsText.setPosition(m_Window->getSize().x / 2 - m_soundEffectsText.getGlobalBounds().width + 165.f, m_musicSlider.getPosition().y + 46);
    m_soundEffectsText.setFillColor(sf::Color(97, 24, 24));

    m_soundEffectsSlider.setSize(sf::Vector2f(500, 10));
    m_soundEffectsSlider.setFillColor(sf::Color(163, 163, 163));
    m_soundEffectsSlider.setPosition((m_Window->getSize().x - m_soundEffectsSlider.getSize().x) / 2, m_soundEffectsText.getPosition().y + m_soundEffectsText.getGlobalBounds().height + 45);

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

    m_debugModeText.setFont(g_fSingleton.getFredokaOne());
    m_debugModeText.setCharacterSize(40);
    m_debugModeText.setString("Debug-Mode: ");
    m_debugModeText.setPosition(m_musicSlider.getPosition().x, m_soundEffectsSlider.getPosition().y + m_soundEffectsSlider.getGlobalBounds().height + 55.f);
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

    m_backBtn.getSprite().setPosition(20, m_Window->getSize().y - m_backBtn.getSprite().getGlobalBounds().height - 20);
}
