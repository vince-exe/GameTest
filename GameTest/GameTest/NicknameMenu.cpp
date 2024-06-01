#include "NicknameMenu.h"

std::string NicknameMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, PopupReturnValues& checker) {
	this->windowPtr = windowPtr;

    setTextures();
    initSprites();
    
    bool requestExit = false;
    sf::Event event;
    while (windowPtr->isOpen() && !requestExit) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                checker = PopupReturnValues::BACK;
                return "";
            }
            handleTextEntered(event);
            handleMouseButtons(event, checker, requestExit);
        }
        draw();
    }

    return inputText;
}

void NicknameMenu::draw() {
	windowPtr->clear();
    windowPtr->draw(text);
    windowPtr->draw(cancelBtn);
    windowPtr->draw(doneBtn);

    inputDisplay.setString(inputText);
    windowPtr->draw(inputDisplay);
    windowPtr->draw(line);

	windowPtr->display();
}

void NicknameMenu::setTextures() {
    this->text.setTexture(MainMenuTextureManager::nicknameText);
    this->doneBtn.setTexture(MainMenuTextureManager::doneText);
    this->cancelBtn.setTexture(MainMenuTextureManager::cancelText);
}

void NicknameMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;
    float windowYSize = windowPtr->getSize().y;

    text.getSprite().setPosition((windowXSize - text.getTexture().getSize().x) / 2, (windowYSize / 2) - 220.f);

    line.setSize(sf::Vector2f(text.getSprite().getGlobalBounds().width, 5));
    line.setFillColor(sf::Color(163, 163, 163));
    line.setRotation(0);
    line.setPosition(text.getSprite().getPosition().x, text.getSprite().getPosition().y + 240);

    inputDisplay.setFont(FontManager::fredokaOne);
    inputDisplay.setCharacterSize(45);

    cancelBtn.getSprite().setPosition((windowXSize - cancelBtn.getTexture().getSize().x) / 2 - 230, line.getPosition().y + 100);
    doneBtn.getSprite().setPosition((windowXSize - doneBtn.getTexture().getSize().x) / 2 + 230, line.getPosition().y + 100);
}

void NicknameMenu::handleTextEntered(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        /* check the size of the nickname */
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && inputText.size() > 0) {
                inputText.pop_back();
            }
            else if (event.text.unicode != '\b' && inputText.length() < 14) {
                inputText += event.text.unicode;

                inputDisplay.setString(inputDisplay.getString() + static_cast<char>(event.text.unicode));
                /* set the text at the center of the line */
                sf::FloatRect textBounds = inputDisplay.getLocalBounds();
                inputDisplay.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
                inputDisplay.setPosition(line.getPosition().x + line.getSize().x / 2.f, (line.getPosition().y - textBounds.height / 2.f) - 8.f);
            }
        }
    }
}

void NicknameMenu::handleMouseButtons(sf::Event& event, PopupReturnValues& checker, bool& exitRequested) {
    sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (cancelBtn.isInside(position)) {
            checker = PopupReturnValues::BACK;
            exitRequested = true;
        }
        else if (doneBtn.isInside(position) && inputText.size()) {
            checker = PopupReturnValues::DONE;
            exitRequested = true;
        }
    }
}
