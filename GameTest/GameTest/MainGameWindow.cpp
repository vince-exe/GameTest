#include "MainGameWindow.h"

void MainGameWindow::init() {
	// this->windowPtr = windowPtr;
    this->windowPtr = std::make_shared<sf::RenderWindow>();
    this->windowPtr->create(sf::VideoMode::getDesktopMode(), "SkyFall Showdown", sf::Style::None);
    this->windowPtr->setFramerateLimit(60);

    sf::Event event;

    setTextures();
    initSprites();

    while (windowPtr->isOpen()) {
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                ;
            }
        }
        draw();
    }
}

void MainGameWindow::draw() {
    this->windowPtr->clear();

    this->windowPtr->draw(background);
    this->windowPtr->draw(line);

    this->windowPtr->display();
}

void MainGameWindow::setTextures() {
    this->background.setTexture(MainGameTextureManager::background);
}

void MainGameWindow::initSprites() {
    line.setSize(sf::Vector2f(12, windowPtr->getSize().y - 200));
    line.setFillColor(sf::Color(237, 237, 237, 215));
    line.setPosition(windowPtr->getSize().x / 2, 0);
}
