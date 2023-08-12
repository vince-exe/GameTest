#include "MainWindow.h"

MainWindow::MainWindow() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::Fullscreen);
    this->windowPtr->setFramerateLimit(60);
}

bool MainWindow::loadTextures() {
    return backgroundTexture.loadFromFile("assets/Background.png") && playBtnText.loadFromFile("assets/playBtn.png")
        && settingBtnText.loadFromFile("assets/settingsBtn.png") && exitBtnText.loadFromFile("assets/exitBtn.png")
        && cursorTextureGrab.loadFromFile("assets/Cursor_Grab.png");
}

bool MainWindow::loadMouse() {
    if (customCursor.loadFromPixels(cursorTextureGrab.copyToImage().getPixelsPtr(), cursorTextureGrab.getSize(), { 0, 0 })) {
        windowPtr->setMouseCursor(customCursor);
        return true;
    }

    return false;
}

void MainWindow::init() {
    initSprites();

    while (windowPtr->isOpen()) {
        sf::Event event;
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
        }

        windowPtr->clear();

        windowPtr->draw(backgroundSprite);
        windowPtr->draw(playBtnSprite);
        windowPtr->draw(settingBtnSprite);
        windowPtr->draw(exitBtnSprite);

        windowPtr->display();
    }
}

void MainWindow::initSprites() {
    backgroundSprite.setTexture(backgroundTexture);

    playBtnSprite.setTexture(playBtnText);
    playBtnSprite.setPosition(800.f, 345.f);

    settingBtnSprite.setTexture(settingBtnText);
    settingBtnSprite.setPosition(800.f, 465.f);

    exitBtnSprite.setTexture(exitBtnText);
    exitBtnSprite.setPosition(800.f, 585);
}
