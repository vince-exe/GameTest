#include "MainMenu.h"

MainMenu::MainMenu() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::None);
    this->windowPtr->setFramerateLimit(60);
}

bool MainMenu::loadTextures() {
    return background.loadTexture("assets/Background.png") && playBtn.loadTexture("assets/playBtn.png")
        && settingsBtn.loadTexture("assets/settingsBtn.png") && exitBtn.loadTexture("assets/exitBtn.png")
        && cursorTextureGrab.loadFromFile("assets/Cursor_Grab.png") && cursorTexturePoint.loadFromFile("assets/Cursor_Point.png");
}

bool MainMenu::loadMouse() {
    if (!defaultCursor.loadFromPixels(cursorTextureGrab.copyToImage().getPixelsPtr(), cursorTextureGrab.getSize(), { 0, 0 }) ||
        !pointCursor.loadFromPixels(cursorTexturePoint.copyToImage().getPixelsPtr(), cursorTexturePoint.getSize(), { 0, 0 })
       ) {
        return false;
    }
    
    windowPtr->setMouseCursor(defaultCursor);
    
    return true;
}

bool MainMenu::init() {
    if (!loadTextures()) {
        std::cout << "\n[ Error ]: Failed to load some / all Game's textures ( MainMenu )";
        false;
    }
    if (!loadMouse()) {
        std::cout << "\n[ Error ]: Failed to load the cursor..";
        false;
    }

    initSprites();
    sf::Event event;

    bool exitRequested = false;
    while (windowPtr->isOpen() && !exitRequested) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            handleMouseCursor(event);

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                handleButtonClicks(event, exitRequested);
            }
        }
        renderWindow();
    }

    return (exitRequested == false);
}

void MainMenu::initSprites() {
    playBtn.getSprite().setPosition(800.f, 345.f);

    settingsBtn.getSprite().setPosition(800.f, 465.f);

    exitBtn.getSprite().setPosition(800.f, 585);
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

        if (playBtn.isInside(position)) {

        }
        else if (settingsBtn.isInside(position)) {

        }
        else if (exitBtn.isInside(position)) {
            /* handle a new window */
            MenuConfirmationExit menuConfirmationExit;
            MenuConfirmationExitValues checker{};

            menuConfirmationExit.init(windowPtr, background, checker, defaultCursor, pointCursor);

            if (checker == MenuConfirmationExitValues::TEXTURE_FAIL) {
                std::cout << "\n[ Error ]: Failed to load some / all Game's textures ( MenuConfirmationExit )";
                exitRequested = true;
            }
            else if (checker == MenuConfirmationExitValues::EXIT) {
                exitRequested = true;
            }
        }
    }
}