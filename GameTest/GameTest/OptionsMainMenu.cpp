#include "OptionsMainMenu.h"

void OptionsMainMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, std::shared_ptr<Music> backgroundMusicPtr, Entity& background, PopupReturnValues& checker) {
    if (!loadTextures()) {
        checker = PopupReturnValues::TEXTURE_FAIL;
        return;
    }
    this->windowPtr = windowPtr;

    sf::Cursor defaultCursor;
    defaultCursor.loadFromSystem(sf::Cursor::Arrow);
    windowPtr->setMouseCursor(defaultCursor);
    
    initSprites(backgroundMusicPtr);

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
            handleMouseButtons(backgroundMusicPtr, event, requestExit, checker);
        }

        draw(background);
    }
}

void OptionsMainMenu::draw(Entity& background) {
    windowPtr->clear();
    windowPtr->draw(background);
    windowPtr->draw(backBtn);
    windowPtr->draw(slider);
    windowPtr->draw(volumeText);
   
    for (int i = 0; i < 10; i++) {
        windowPtr->draw(checkPoints[i]);
        windowPtr->draw(volumeLevelText[i]);
    }
    windowPtr->display();
}

void OptionsMainMenu::handleMouseButtons(std::shared_ptr<Music> backgroundMusicPtr, sf::Event& event, bool& requestExit, PopupReturnValues& checker) {
    sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        checkVolumeLevel(backgroundMusicPtr, position);

        if (backBtn.isInside(position)) {
            requestExit = true;
            checker = PopupReturnValues::BACK;
            return;
        }
    }
}

void OptionsMainMenu::checkVolumeLevel(std::shared_ptr<Music> backgroundMusicPtr, sf::Vector2f& position) {
    int volumeLevel = 0;

    sf::Vector2i mousePosition = sf::Mouse::getPosition(*windowPtr);
    for (int i = 0; i < 10; i++) {
        if (checkPoints[i].getGlobalBounds().contains(position)) {
            checkPoints[oldVolumeIndex].setFillColor(defCheckpointColor);
            oldVolumeIndex = i;
            
            checkPoints[i].setFillColor(selectedCheckpointColor);
            backgroundMusicPtr->setVolume(volumeLevel);
        }
        volumeLevel += 10;
    }
}

void OptionsMainMenu::initSprites(std::shared_ptr<Music> backgroundMusicPtr) {
    slider.setSize(sf::Vector2f(500, 10));
    slider.setFillColor(sf::Color(255, 51, 51));
    slider.setPosition((windowPtr->getSize().x - slider.getSize().x) / 2, (windowPtr->getSize().y / 2) - 200);

    sf::Vector2u spriteSize = volumeText.getTexture().getSize();
    float x = (windowPtr->getSize().x - spriteSize.x) / 2.0f;
    float y = slider.getPosition().y - 100;
    volumeText.getSprite().setPosition(x, y);

    x = (windowPtr->getSize().x - backBtn.getTexture().getSize().x) / 2.0f;
    backBtn.getSprite().setPosition(x, 700);

    for (int i = 0; i < 10; ++i) {
        checkPoints[i].setSize(sf::Vector2f(15, 50));
        checkPoints[i].setFillColor(defCheckpointColor);
        checkPoints[i].setPosition(sf::Vector2f(slider.getPosition().x + i * (slider.getSize().x / 9), slider.getPosition().y - 21));
        
        volumeLevelText[i].getSprite().setPosition(checkPoints[i].getPosition().x - 10, checkPoints[i].getPosition().y + 60);
    } 
    /* set the specif position of the 0 */
    volumeLevelText[0].getSprite().setPosition(checkPoints[0].getPosition().x - 5, checkPoints[0].getPosition().y + 60);

    /* set the current level of music*/
    int volume = backgroundMusicPtr->getVolume();
    if ( volume == 0) {
        checkPoints[0].setFillColor(selectedCheckpointColor);
        this->oldVolumeIndex = 0;
    }
    else {
        checkPoints[volume / 10].setFillColor(selectedCheckpointColor);
        this->oldVolumeIndex = volume / 10;
    }
}

bool OptionsMainMenu::loadTextures() {
    /* load the volume numbers */
    int pathNumber = 0;
    std::string pathStr;
    for (int i = 0; i < 10; i++) {
        pathStr = "assets/" + std::to_string(pathNumber) + "_Text.png";
        if (!volumeLevelText[i].loadTexture(pathStr)) { return false; }
        pathNumber += 10;
    }

    return volumeText.loadTexture("assets/Volume_Png.png") && backBtn.loadTexture("assets/Back_Button.png");
}
