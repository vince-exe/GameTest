#include "MainWindow.h"

MainWindow::MainWindow() {
    this->windowPtr = std::make_shared<sf::RenderWindow>();

	this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::Fullscreen);
}

bool MainWindow::initTextures() {
    return backgroundTexture.loadFromFile("assets/Background.png");
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

        windowPtr->display();
    }
}

void MainWindow::initSprites() {
    backgroundSprite.setTexture(backgroundTexture);
}
