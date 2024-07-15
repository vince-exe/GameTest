#include "IpPortMenu.h"

std::pair<std::string, int> IpPortMenu::init(sf::RenderWindow& window, TextureManager& textureManager, FontManager& fontManager, SettingsManager& settingsManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker) {
    m_Window = &window; 
    
    setTextures(textureManager);
    initSprites(fontManager);
    setPlaceholder(settingsManager);

    bool requestExit = false;
    sf::Event event;

    while (m_Window->isOpen() && !requestExit) {
        while (m_Window->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                m_Window->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                checker = SkyfallUtils::WindowsReturnValues::BACK;
                return std::pair<std::string, int>();
            }
            
            handleTextEntered(event);
            handleMouseButtons(event, settingsManager, audioManager, checker, requestExit);
        }
        draw();
    }
    
    return m_Pair;
}

void IpPortMenu::setTextures(TextureManager& textureManager) {
    m_Text.setTexture(textureManager.getTextImage(5));
    m_connectBtn.setTexture(textureManager.getDoneBtn());
    m_cancelBtn.setTexture(textureManager.getCancelBtn());
    m_entityToDisplay.setTexture(textureManager.getTextImage(7));
}

void IpPortMenu::initSprites(FontManager& fontManager) {
    float windowXSize = m_Window->getSize().x;
    float windowYSize = m_Window->getSize().y;

    m_Text.getSprite().setPosition((windowXSize - m_Text.getTexture().getSize().x) / 2, (windowYSize / 2) - 250.f);

    m_Line.setSize(sf::Vector2f(m_Text.getSprite().getGlobalBounds().width, 5));
    m_Line.setFillColor(sf::Color(163, 163, 163));
    m_Line.setRotation(0);
    m_Line.setPosition(m_Text.getSprite().getPosition().x, m_Text.getSprite().getPosition().y + 240);

    m_inputDisplay.setFont(fontManager.getFredokaOne());
    m_inputDisplay.setCharacterSize(45);

    m_cancelBtn.getSprite().setPosition((windowXSize - m_cancelBtn.getTexture().getSize().x) / 2 - 230, m_Line.getPosition().y + 100);
    m_connectBtn.getSprite().setPosition((windowXSize - m_connectBtn.getTexture().getSize().x) / 2 + 230, m_Line.getPosition().y + 100);
}

void IpPortMenu::setPlaceholder(SettingsManager& settingsManager) {
    std::string ipPort = settingsManager.getValue(SkyfallUtils::Settings::DEFAULT_NETWORK).GetString();

    for (int i = 0; i < ipPort.length(); i++) {
        m_Pair.first += ipPort[i];

        m_inputDisplay.setString(m_inputDisplay.getString() + static_cast<char>(ipPort[i]));

        /* set the m_Text at the center of the m_Line */
        sf::FloatRect m_TextBounds = m_inputDisplay.getLocalBounds();
        m_inputDisplay.setOrigin(m_TextBounds.left + m_TextBounds.width / 2.f, m_TextBounds.top + m_TextBounds.height / 2.f);
        m_inputDisplay.setPosition(m_Line.getPosition().x + m_Line.getSize().x / 2.f, (m_Line.getPosition().y - m_TextBounds.height / 2.f) - 8.f);
    }
}

void IpPortMenu::draw() {
    m_Window->clear();
    m_Window->draw(m_Text);
    m_Window->draw(m_cancelBtn);
    m_Window->draw(m_connectBtn);

    m_inputDisplay.setString(m_Pair.first);
    m_Window->draw(m_inputDisplay);
    m_Window->draw(m_Line);

    /* display the notification message from the server */
    if (m_displayText) {
        m_Window->draw(*m_msgToDisplay);
    }

    m_Window->display();
}

void IpPortMenu::handleTextEntered(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        /* check the size of the nickname */
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && m_Pair.first.size() > 0) {
                m_Pair.first.pop_back();
            }
            else {
                m_Pair.first += event.text.unicode;

                m_inputDisplay.setString(m_inputDisplay.getString() + static_cast<char>(event.text.unicode));
                /* set the m_Text at the center of the m_Line */
                sf::FloatRect m_TextBounds = m_inputDisplay.getLocalBounds();
                m_inputDisplay.setOrigin(m_TextBounds.left + m_TextBounds.width / 2.f, m_TextBounds.top + m_TextBounds.height / 2.f);
                m_inputDisplay.setPosition(m_Line.getPosition().x + m_Line.getSize().x / 2.f, (m_Line.getPosition().y - m_TextBounds.height / 2.f) - 8.f);
            }
        }
    }
}

void IpPortMenu::handleMouseButtons(sf::Event& event, SettingsManager& settingsManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker, bool& exitRequested) {
    sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (m_cancelBtn.isInside(position)) {
            audioManager.getButtonClickSound();
            checker = SkyfallUtils::WindowsReturnValues::BACK;
            exitRequested = true;
        }
        else if (m_connectBtn.isInside(position)) {
            /* check if the format is valid */
            std::string check = m_Pair.first;
            if (setIpPort(check)) {
                audioManager.getButtonClickSound().play();
                settingsManager.setString_("DefaultIpPort", check);
                checker = SkyfallUtils::WindowsReturnValues::DONE;
                exitRequested = true;
            }
            else {
                audioManager.getErrorSound().play();
                displayTextFuncTime(m_entityToDisplay, 5);
            }
        }
    }
}

bool IpPortMenu::setIpPort(std::string ipPort) {
    size_t pos = ipPort.find(":");

    if (pos != std::string::npos) {
        m_Pair.first = ipPort.substr(0, pos);
        m_Pair.second = std::stoi(ipPort.substr(pos + 1));

        return true;
    }
    else {
        return false;
    }
}

void IpPortMenu::displayTextFuncTime(Entity& entity, int seconds) {
    std::thread t([this, &entity, seconds]() {
        using namespace std::chrono_literals;

        m_msgToDisplay = &entity;
        m_msgToDisplay->getSprite().setPosition((m_Window->getSize().x - entity.getTexture().getSize().x) / 2, 530);
        m_displayText = true;

        for (int i = 0; i < seconds; i++) {
            std::this_thread::sleep_for(1s);
        }
        m_displayText = false;
        });
    t.detach();
}