#include "OptionsMainMenu.h"

void OptionsMainMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background) {
    sf::Cursor defaultCursor;
    defaultCursor.loadFromSystem(sf::Cursor::Arrow);
    windowPtr->setMouseCursor(defaultCursor);

    slider.setSize(sf::Vector2f(500, 10));
    slider.setFillColor(sf::Color(255, 51, 51));
    slider.setPosition((windowPtr->getSize().x - slider.getSize().x) / 2, windowPtr->getSize().y / 2);

    for (int i = 0; i < 10; ++i) {
        checkPoints[i].setSize(sf::Vector2f(15, 50));
        checkPoints[i].setFillColor(sf::Color(255, 51, 51));
        checkPoints[i].setPosition(sf::Vector2f(slider.getPosition().x + i * (slider.getSize().x / 9), slider.getPosition().y - 21));
    }
    checkPoints[4].setFillColor(sf::Color(58, 59, 60));

    while (windowPtr->isOpen()) {
        sf::Event event;
        while (windowPtr->pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                windowPtr->close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*windowPtr);
                for (int i = 0; i < 10; i++) {
                    if (checkPoints[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                        std::cout << "\nentrato nel n: " << i + 1;
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                return;
            }
        }

        draw(windowPtr, background);
    }
}

void OptionsMainMenu::draw(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background) {
    windowPtr->clear();
    windowPtr->draw(background);
    windowPtr->draw(slider);
    
    for (int i = 0; i < 10; i++) {
        windowPtr->draw(checkPoints[i]);
    }
    windowPtr->display();
}