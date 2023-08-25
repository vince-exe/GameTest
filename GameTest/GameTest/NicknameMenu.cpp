#include "NicknameMenu.h"

void NicknameMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupReturnValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor) {
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
                return;
            }
            handleTextEntered(event);
            handleMouseCursor(pointCursor, defCursor);
        }

        draw(background);
    }
}

void NicknameMenu::draw(Entity& background) {
	windowPtr->clear();
	windowPtr->draw(background);
    windowPtr->draw(text);
    windowPtr->draw(backBtn);
    windowPtr->draw(doneBtn);

    inputDisplay.setString(inputText);
    windowPtr->draw(inputDisplay);
    windowPtr->draw(line);

	windowPtr->display();
}

void NicknameMenu::setTextures() {
    this->text.setTexture(MainMenuTextureManager::nicknameText);
    this->doneBtn.setTexture(MainMenuTextureManager::doneBtn);
    this->backBtn.setTexture(MainMenuTextureManager::backBtn);
}

void NicknameMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;
    float windowYSize = windowPtr->getSize().y;

    text.getSprite().setPosition((windowXSize - text.getTexture().getSize().x) / 2, (windowYSize / 2) - 250.f);

    line.setSize(sf::Vector2f(text.getSprite().getGlobalBounds().width, 5));
    line.setFillColor(sf::Color(255, 51, 51));
    line.setRotation(0);
    line.setPosition(text.getSprite().getPosition().x, text.getSprite().getPosition().y + 240);

    inputDisplay.setFont(FontManager::fredokaOne);
    inputDisplay.setCharacterSize(45);

    backBtn.getSprite().setPosition((windowXSize - backBtn.getTexture().getSize().x) / 2 - 230, line.getPosition().y + 100);
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

void NicknameMenu::handleMouseCursor(sf::Cursor& pointCursor, sf::Cursor& defCursor) {
    sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

    if (backBtn.isInside(position) || doneBtn.isInside(position)) {
        windowPtr->setMouseCursor(pointCursor);
    }
    else {
        windowPtr->setMouseCursor(defCursor);
    }
}
