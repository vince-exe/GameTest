#include "MainMenu.h"

MainMenu::MainMenu() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->backgroundMusicPtr = std::make_shared<Music>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::None);
    this->windowPtr->setFramerateLimit(60);
}

void MainMenu::setTextures() {
    background.setTexture(MainMenuTextureManager::background);
    playBtn.setTexture(MainMenuTextureManager::playBtn);
    settingsBtn.setTexture(MainMenuTextureManager::settingsBtn);
    exitBtn.setTexture(MainMenuTextureManager::exitBtn);
}

bool MainMenu::loadMouse() {
    if (!defaultCursor.loadFromPixels(MainMenuTextureManager::cursorTextureGrab.copyToImage().getPixelsPtr(), MainMenuTextureManager::cursorTextureGrab.getSize(), { 0, 0 }) ||
        !pointCursor.loadFromPixels(MainMenuTextureManager::cursorTexturePoint.copyToImage().getPixelsPtr(), MainMenuTextureManager::cursorTexturePoint.getSize(), { 0, 0 })) {
        return false;
    }
    
    windowPtr->setMouseCursor(defaultCursor);
    return true;
}

bool MainMenu::loadMusic() {
    return backgroundMusicPtr->getMusic().openFromFile("assets/Background_Menu_Music.ogg");
}

bool MainMenu::init() {
    if (!loadMouse()) {
        std::cout << "\n[ Error ]: Failed to load the cursor ( MainMenu )\n";
        return false;
    }
    if (!loadMusic()) {
        std::cout << "\n[ Error ]: Failed to load the music ( MainMenu )\n";
        return false;
    }
    backgroundMusicPtr->setVolume(60.f);
    backgroundMusicPtr->play();
    backgroundMusicPtr->loop(true);

    setTextures();
    initSprites();
    sf::Event event;

    bool exitRequested = false;
    while (windowPtr->isOpen() && !exitRequested) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            /* OPEN THE EXIT MENU */
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                MenuConfirmationExit menuConfirmationExit;
                PopupReturnValues checker{};

                menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);
                if (checker == PopupReturnValues::EXIT) {
                    exitRequested = true;
                }
            }
            handleMouseCursor(event);
            handleButtonClicks(event, exitRequested);
        }
        renderWindow();
    }

    backgroundMusicPtr->getMusic().stop();
    return (exitRequested == false);
}

void MainMenu::initSprites() {
    float windowXSize = windowPtr->getSize().x;

    playBtn.getSprite().setPosition((windowXSize - playBtn.getTexture().getSize().x) / 2, 345.f);
    settingsBtn.getSprite().setPosition((windowXSize - settingsBtn.getTexture().getSize().x) / 2, 465.f);
    exitBtn.getSprite().setPosition((windowXSize - exitBtn.getTexture().getSize().x) / 2, 585.f);
}

void MainMenu::renderWindow() {
    windowPtr->clear();
    
    windowPtr->draw(background);
    windowPtr->draw(playBtn);
    windowPtr->draw(settingsBtn);
    windowPtr->draw(exitBtn);

    windowPtr->display();
}

void MainMenu::handleMouseCursor(sf::Event& event) {
    /* change the mouse cursor from handle to point if it is on a button */
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

        if (playBtn.isInside(position) || settingsBtn.isInside(position) || exitBtn.isInside(position)) {
            windowPtr->setMouseCursor(pointCursor);
        }
        else {
            windowPtr->setMouseCursor(defaultCursor);
        }
    }
}

void MainMenu::handleButtonClicks(sf::Event& event, bool& exitRequested) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

        /* OPEN THE NICKNAME MENU */
        if (playBtn.isInside(position)) {
            NicknameMenu nicknameMenu;
            PopupReturnValues checker{};

            nicknameMenu.init(windowPtr, background, checker, defaultCursor, pointCursor);
        }
        /* SETTINGS MENU */
        else if (settingsBtn.isInside(position)) {
            OptionsMainMenu optionsMainMenu;
            PopupReturnValues checker{};

            optionsMainMenu.init(windowPtr, backgroundMusicPtr, background, checker);
        }
        /* EXIT MENU */
        else if (exitBtn.isInside(position)) {
            MenuConfirmationExit menuConfirmationExit;
            PopupReturnValues checker{};

            menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);
            if (checker == PopupReturnValues::EXIT) {
                exitRequested = true;
            }
        }
    }
}
