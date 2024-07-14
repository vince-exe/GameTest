#include "OptionsMainMenu.h"

void OptionsMainMenu::init(sf::RenderWindow& window, TextureManager& textureManager, SettingsManager& settingsManager, FontManager& fontManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker) {
    m_Window = &window;

    sf::Cursor defaultCursor;
    defaultCursor.loadFromSystem(sf::Cursor::Arrow);
    m_Window->setMouseCursor(defaultCursor);
    
    setTextures(textureManager);
    initSprites(fontManager, audioManager);

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
            handleMouseButtons(event, requestExit, settingsManager, audioManager, checker);
        }
        draw();
    }
}

void OptionsMainMenu::draw() {
    m_Window->clear();
    m_Window->draw(m_backBtn);
    m_Window->draw(m_Slider);
    m_Window->draw(m_musicText);
   
    for (int i = 0; i < 10; i++) {
        m_Window->draw(m_checkPoints[i]);
    }
    m_Window->display();
}

void OptionsMainMenu::handleMouseButtons(sf::Event& event, bool& requestExit, SettingsManager& settingsManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker) {
    sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        checkVolumeLevel(settingsManager, audioManager, position);

        if (m_backBtn.isInside(position)) {
            requestExit = true;
            checker = SkyfallUtils::WindowsReturnValues::BACK;
            return;
        }
    }
}

void OptionsMainMenu::checkVolumeLevel(SettingsManager& settingsManager, AudioManager& audioManager, sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_Window);
    for (int i = 0; i < 10; i++) {
        if (m_checkPoints[i].getGlobalBounds().contains(position)) {
            m_checkPoints[m_oldVolumeIndex].setFillColor(m_defCheckpointColor);
            m_oldVolumeIndex = i;
            m_checkPoints[i].setFillColor(m_selectedCheckpointColor);
            audioManager.getBackgroundMusic().setVolume(volumeLevel);
            settingsManager.setInt_(SkyfallUtils::Settings::MUSIC_VOLUME, volumeLevel);
        } 
        volumeLevel += 10;
    }
}

void OptionsMainMenu::setTextures(TextureManager& textureManager) {
    m_backBtn.setTexture(textureManager.getCancelBtn());
}

void OptionsMainMenu::initSprites(FontManager& fontManager, AudioManager& audioManager) {
    m_Slider.setSize(sf::Vector2f(500, 10));
    m_Slider.setFillColor(sf::Color(163, 163, 163));
    m_Slider.setPosition((m_Window->getSize().x - m_Slider.getSize().x) / 2, m_Slider.getGlobalBounds().height + 105);

    m_backBtn.getSprite().setPosition(20, m_Window->getSize().y - m_backBtn.getSprite().getGlobalBounds().height - 20);

    m_musicText.setFont(fontManager.getFredokaOne());
    m_musicText.setCharacterSize(50);
    m_musicText.setString("Music");
    m_musicText.setPosition(m_Window->getSize().x / 2 - m_musicText.getGlobalBounds().width + 70.f, 20.f);

    for (int i = 0; i < 10; ++i) {
        m_checkPoints[i].setSize(sf::Vector2f(15, 50));
        m_checkPoints[i].setFillColor(m_defCheckpointColor);
        m_checkPoints[i].setPosition(sf::Vector2f(m_Slider.getPosition().x + i * (m_Slider.getSize().x / 9), m_Slider.getPosition().y - 21));
    } 

    /* set the current level of music*/
    int volume = audioManager.getBackgroundMusic().getVolume();
    if ( volume == 0) {
        m_checkPoints[0].setFillColor(m_selectedCheckpointColor);
        m_oldVolumeIndex = 0;
    }
    else {
        m_checkPoints[volume / 10].setFillColor(m_selectedCheckpointColor);
        m_oldVolumeIndex = volume / 10;
    }
}
