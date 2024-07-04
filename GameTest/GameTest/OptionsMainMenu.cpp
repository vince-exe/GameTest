#include "OptionsMainMenu.h"

void OptionsMainMenu::init(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<Music> backgroundMusicPtr, UiUtils::WindowsReturnValues& checker) {
    m_Window = window;

    sf::Cursor defaultCursor;
    defaultCursor.loadFromSystem(sf::Cursor::Arrow);
    m_Window->setMouseCursor(defaultCursor);
    
    setTextures();
    initSprites(backgroundMusicPtr);

    bool requestExit = false;
    sf::Event event;
    while (m_Window->isOpen() && !requestExit) {
        while (m_Window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_Window->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                checker = UiUtils::WindowsReturnValues::BACK;
                return;
            }
            handleMouseButtons(backgroundMusicPtr, event, requestExit, checker);
        }
        draw();
    }
}

void OptionsMainMenu::draw() {
    m_Window->clear();
    m_Window->draw(m_backBtn);
    m_Window->draw(m_Slider);
    m_Window->draw(m_volumeText);
   
    for (int i = 0; i < 10; i++) {
        m_Window->draw(m_checkPoints[i]);
    }
    m_Window->display();
}

void OptionsMainMenu::handleMouseButtons(std::shared_ptr<Music> backgroundMusicPtr, sf::Event& event, bool& requestExit, UiUtils::WindowsReturnValues& checker) {
    sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        checkVolumeLevel(backgroundMusicPtr, position);

        if (m_backBtn.isInside(position)) {
            requestExit = true;
            checker = UiUtils::WindowsReturnValues::BACK;
            return;
        }
    }
}

void OptionsMainMenu::checkVolumeLevel(std::shared_ptr<Music> backgroundMusicPtr, sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_Window);
    for (int i = 0; i < 10; i++) {
        if (m_checkPoints[i].getGlobalBounds().contains(position)) {
            m_checkPoints[m_oldVolumeIndex].setFillColor(m_defCheckpointColor);
            m_oldVolumeIndex = i;
            m_checkPoints[i].setFillColor(m_selectedCheckpointColor);
            backgroundMusicPtr->setVolume(volumeLevel);
            SettingsManager::setInt_("VolumeMenu", volumeLevel);
        }
        volumeLevel += 10;
    }
}

void OptionsMainMenu::setTextures() {
    m_volumeText.setTexture(MainMenuTextureManager::optionsTextVolume);
    m_backBtn.setTexture(MainMenuTextureManager::cancelText);
}

void OptionsMainMenu::initSprites(std::shared_ptr<Music> backgroundMusicPtr) {
    m_Slider.setSize(sf::Vector2f(500, 10));
    m_Slider.setFillColor(sf::Color(163, 163, 163));
    /* center the sprite */
    m_Slider.setPosition((m_Window->getSize().x - m_Slider.getSize().x) / 2, m_Slider.getGlobalBounds().height + 105);

    sf::Vector2u spriteSize = m_volumeText.getTexture().getSize();
    float x = (m_Window->getSize().x - spriteSize.x) / 2.0f;
    float y = m_Slider.getPosition().y - 100;
    m_volumeText.getSprite().setPosition(x, 30);

    x = (m_Window->getSize().x - m_backBtn.getTexture().getSize().x) / 2.0f;
    m_backBtn.getSprite().setPosition(20, m_Window->getSize().y - m_backBtn.getSprite().getGlobalBounds().height - 20);

    for (int i = 0; i < 10; ++i) {
        m_checkPoints[i].setSize(sf::Vector2f(15, 50));
        m_checkPoints[i].setFillColor(m_defCheckpointColor);
        m_checkPoints[i].setPosition(sf::Vector2f(m_Slider.getPosition().x + i * (m_Slider.getSize().x / 9), m_Slider.getPosition().y - 21));
    } 

    /* set the current level of music*/
    int volume = backgroundMusicPtr->getVolume();
    if ( volume == 0) {
        m_checkPoints[0].setFillColor(m_selectedCheckpointColor);
        m_oldVolumeIndex = 0;
    }
    else {
        m_checkPoints[volume / 10].setFillColor(m_selectedCheckpointColor);
        m_oldVolumeIndex = volume / 10;
    }
}
