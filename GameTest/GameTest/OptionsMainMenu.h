#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"

class OptionsMainMenu {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background);

private:
	void draw(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background);

private:
	sf::RectangleShape slider;

	sf::RectangleShape checkPoints[10];
};

