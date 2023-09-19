#include "IpPortMenu.h"

std::pair<std::string, int> IpPortMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, std::shared_ptr<Sound> notificationSound, Entity& background, PopupReturnValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor) {
    this->windowPtr = windowPtr;
    
    setTextures();
    initSprites();
    setPlaceholder();

    bool requestExit = false;
    sf::Event event;

    while (windowPtr->isOpen() && !requestExit) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                checker = PopupReturnValues::BACK;
                return std::pair<std::string, int>();
            }
            
            handleTextEntered(event);
            handleMouseCursor(pointCursor, defCursor);
            handleMouseButtons(event, checker, requestExit, notificationSound);
        }
        draw(background);
    }
    
    return pair;
}

void IpPortMenu::setTextures() {
    this->text.setTexture(MainMenuTextureManager::connectText);
    this->connectBtn.setTexture(MainMenuTextureManager::connectBtn);
    this->cancelBtn.setTexture(MainMenuTextureManager::cancelBtn);
    this->entityToDisplay.setTexture(MainMenuTextureManager::invalidFormatText);
}

void IpPortMenu::setPlaceholder() {
    std::string ipPort = SettingsManager::getValue("DefaultIpPort").GetString();

    for (int i = 0; i < ipPort.length(); i++) {
        pair.first += ipPort[i];

        inputDisplay.setString(inputDisplay.getString() + static_cast<char>(ipPort[i]));

        /* set the text at the center of the line */
        sf::FloatRect textBounds = inputDisplay.getLocalBounds();
        inputDisplay.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
        inputDisplay.setPosition(line.getPosition().x + line.getSize().x / 2.f, (line.getPosition().y - textBounds.height / 2.f) - 8.f);
    }
}

void IpPortMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;
    float windowYSize = windowPtr->getSize().y;

    text.getSprite().setPosition((windowXSize - text.getTexture().getSize().x) / 2, (windowYSize / 2) - 250.f);

    line.setSize(sf::Vector2f(text.getSprite().getGlobalBounds().width, 5));
    line.setFillColor(sf::Color(255, 51, 51));
    line.setRotation(0);
    line.setPosition(text.getSprite().getPosition().x, text.getSprite().getPosition().y + 240);

    inputDisplay.setFont(FontManager::fredokaOne);
    inputDisplay.setCharacterSize(45);

    cancelBtn.getSprite().setPosition((windowXSize - cancelBtn.getTexture().getSize().x) / 2 - 230, line.getPosition().y + 100);
    connectBtn.getSprite().setPosition((windowXSize - connectBtn.getTexture().getSize().x) / 2 + 230, line.getPosition().y + 100);
}

void IpPortMenu::draw(Entity& background) {
    windowPtr->clear();
    windowPtr->draw(background);
    windowPtr->draw(text);
    windowPtr->draw(cancelBtn);
    windowPtr->draw(connectBtn);

    inputDisplay.setString(pair.first);
    windowPtr->draw(inputDisplay);
    windowPtr->draw(line);

    /* display the notification message from the server */
    if (displayText) {
        windowPtr->draw(*msgToDisplay);
    }

    windowPtr->display();
}

void IpPortMenu::handleTextEntered(sf::Event& event) {
    if (event.type == sf::Event::TextEntered) {
        /* check the size of the nickname */
        if (event.text.unicode < 128) {
            if (event.text.unicode == '\b' && pair.first.size() > 0) {
                pair.first.pop_back();
            }
            else {
                pair.first += event.text.unicode;

                inputDisplay.setString(inputDisplay.getString() + static_cast<char>(event.text.unicode));
                /* set the text at the center of the line */
                sf::FloatRect textBounds = inputDisplay.getLocalBounds();
                inputDisplay.setOrigin(textBounds.left + textBounds.width / 2.f, textBounds.top + textBounds.height / 2.f);
                inputDisplay.setPosition(line.getPosition().x + line.getSize().x / 2.f, (line.getPosition().y - textBounds.height / 2.f) - 8.f);
            }
        }
    }
}

void IpPortMenu::handleMouseCursor(sf::Cursor& pointCursor, sf::Cursor& defCursor) {
    sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

    if (cancelBtn.isInside(position) || connectBtn.isInside(position)) {
        windowPtr->setMouseCursor(pointCursor);
    }
    else {
        windowPtr->setMouseCursor(defCursor);
    }
}

void IpPortMenu::handleMouseButtons(sf::Event& event, PopupReturnValues& checker, bool& exitRequested, std::shared_ptr<Sound> notificationSound) {
    sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (cancelBtn.isInside(position)) {
            checker = PopupReturnValues::BACK;
            exitRequested = true;
        }
        else if (connectBtn.isInside(position)) {
            /* check if the format is valid */
            std::string check = pair.first;
            if (setIpPort(check)) {
                SettingsManager::setString_("DefaultIpPort", check);
                checker = PopupReturnValues::DONE;
                exitRequested = true;
            }
            else {
                notificationSound->play();
                displayTextFuncTime(entityToDisplay, 5);
            }
        }
    }
}

bool IpPortMenu::setIpPort(std::string ipPort) {
    size_t pos = ipPort.find(":");

    if (pos != std::string::npos) {
        pair.first = ipPort.substr(0, pos);
        pair.second = std::stoi(ipPort.substr(pos + 1));

        return true;
    }
    else {
        return false;
    }
}

void IpPortMenu::displayTextFuncTime(Entity& entity, int seconds) {
    std::thread t([this, &entity, seconds]() {
        using namespace std::chrono_literals;

        this->msgToDisplay = &entity;
        this->msgToDisplay->getSprite().setPosition(15.f, 480.f);
        this->displayText = true;

        for (int i = 0; i < seconds; i++) {
            std::this_thread::sleep_for(1s);
        }
        displayText = false;
        });
    t.detach();
}