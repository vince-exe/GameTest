#include "MainWindow.h"

MainWindow::MainWindow() {
    this->windowPtr = std::make_shared<sf::Window>();

	this->windowPtr->create(sf::VideoMode(1200, 600), "SkyFall Showdown", sf::Style::Fullscreen | sf::Style::Close);
}

void MainWindow::init() {
    while (windowPtr->isOpen()) {
        sf::Event event;
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
        }
    }
}
