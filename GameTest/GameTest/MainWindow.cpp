#include "MainWindow.h"

MainWindow::MainWindow() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::None);
    this->windowPtr->setFramerateLimit(60);
}

bool MainWindow::loadTextures() {
    return background.loadTexture("assets/Background.png") && playBtn.loadTexture("assets/playBtn.png")
        && settingsBtn.loadTexture("assets/settingsBtn.png") && exitBtn.loadTexture("assets/exitBtn.png")
        && cursorTextureGrab.loadFromFile("assets/Cursor_Grab.png") && cursorTexturePoint.loadFromFile("assets/Cursor_Point.png");
}

bool MainWindow::loadMouse() {
    if (!defaultCursor.loadFromPixels(cursorTextureGrab.copyToImage().getPixelsPtr(), cursorTextureGrab.getSize(), { 0, 0 }) ||
        !pointCursor.loadFromPixels(cursorTexturePoint.copyToImage().getPixelsPtr(), cursorTexturePoint.getSize(), { 0, 0 })
       ) {
        return false;
    }
    
    windowPtr->setMouseCursor(defaultCursor);
    
    return true;
}

void MainWindow::init() {
    initSprites();
    sf::Event event;

    while (windowPtr->isOpen()) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
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

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f position = windowPtr->mapPixelToCoords(sf::Mouse::getPosition(*windowPtr));

                if (playBtn.isInside(position)) {

                }
                else if (settingsBtn.isInside(position)) {

                }
                else if (exitBtn.isInside(position)) {
                    /* handle a new window */
                    PopupExitWindow popupExitWindow;
                    PopupExitWindowValues checker{};

                    popupExitWindow.init(windowPtr, background, checker, defaultCursor, pointCursor);

                    if (checker == PopupExitWindowValues::TEXTURE_FAIL) {
                        std::cout << "\n[ Error ]: Failed to load some / all Game's textures ( PopupExitWindow )";
                        windowPtr->close();
                    }
                    else if(checker == PopupExitWindowValues::EXIT){
                        windowPtr->close();
                    }
                }
            }
        }

        windowPtr->clear();

        windowPtr->draw(background);
        windowPtr->draw(playBtn);
        windowPtr->draw(settingsBtn);
        windowPtr->draw(exitBtn);

        windowPtr->display();
    }
}

void MainWindow::initSprites() {
    playBtn.getSprite().setPosition(800.f, 345.f);

    settingsBtn.getSprite().setPosition(800.f, 465.f);

    exitBtn.getSprite().setPosition(800.f, 585);
}
