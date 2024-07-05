#include "NicknameMenu.h"

std::string NicknameMenu::init(sf::RenderWindow& window, UiUtils::WindowsReturnValues& checker) {
	m_Window = &window;

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
                checker = UiUtils::WindowsReturnValues::BACK;
                return "";
            }
            handleTextEntered(event);
            handleMouseButtons(event, checker, requestExit);
        }
        draw();
    }

    return m_inputText;
}

void NicknameMenu::draw() {
	m_Window->clear();
    m_Window->draw(m_Text);
    m_Window->draw(m_cancelBtn);
    m_Window->draw(m_doneBtn);

    m_inputDisplay.setString(m_inputText);
    m_Window->draw(m_inputDisplay);
    m_Window->draw(m_Line);

	m_Window->display();
}

void NicknameMenu::setTextures() {
    m_Text.setTexture(MainMenuTextureManager::nicknameText);
    m_doneBtn.setTexture(MainMenuTextureManager::doneText);
    m_cancelBtn.setTexture(MainMenuTextureManager::cancelText);
}

void NicknameMenu::initSprites() {
    float windowXSize = m_Window->getSize().x;
    float windowYSize = m_Window->getSize().y;

    m_Text.getSprite().setPosition((windowXSize - m_Text.getTexture().getSize().x) / 2, (windowYSize / 2) - 220.f);

    m_Line.setSize(sf::Vector2f(m_Text.getSprite().getGlobalBounds().width, 5));
    m_Line.setFillColor(sf::Color(163, 163, 163));
    m_Line.setRotation(0);
    m_Line.setPosition(m_Text.getSprite().getPosition().x, m_Text.getSprite().getPosition().y + 240);

    m_inputDisplay.setFont(FontManager::fredokaOne);
    m_inputDisplay.setCharacterSize(45);

    m_cancelBtn.getSprite().setPosition((windowXSize - m_cancelBtn.getTexture().getSize().x) / 2 - 230, m_Line.getPosition().y + 100);
    m_doneBtn.getSprite().setPosition((windowXSize - m_doneBtn.getTexture().getSize().x) / 2 + 230, m_Line.getPosition().y + 100);
}

void NicknameMenu::handleTextEntered(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        /* check the size of the nickname */
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && m_inputText.size() > 0) {
                m_inputText.pop_back();
            }
            else if (event.text.unicode != '\b' && m_inputText.length() < 14) {
                m_inputText += event.text.unicode;

                m_inputDisplay.setString(m_inputDisplay.getString() + static_cast<char>(event.text.unicode));
                /* set the m_Text at the center of the m_Line */
                sf::FloatRect m_TextBounds = m_inputDisplay.getLocalBounds();
                m_inputDisplay.setOrigin(m_TextBounds.left + m_TextBounds.width / 2.f, m_TextBounds.top + m_TextBounds.height / 2.f);
                m_inputDisplay.setPosition(m_Line.getPosition().x + m_Line.getSize().x / 2.f, (m_Line.getPosition().y - m_TextBounds.height / 2.f) - 8.f);
            }
        }
    }
}

void NicknameMenu::handleMouseButtons(sf::Event& event, UiUtils::WindowsReturnValues& checker, bool& exitRequested) {
    sf::Vector2f position = m_Window->mapPixelToCoords(sf::Mouse::getPosition(*m_Window));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (m_cancelBtn.isInside(position)) {
            checker = UiUtils::WindowsReturnValues::BACK;
            exitRequested = true;
        }
        else if (m_doneBtn.isInside(position) && m_inputText.size()) {
            checker = UiUtils::WindowsReturnValues::DONE;
            exitRequested = true;
        }
    }
}
