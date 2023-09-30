#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "MainGameTextureManager.h"

class MainGameWindow {
public:
	void init();

private:
	void draw();

	void setTextures();

	void initSprites();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	Entity background;
	sf::RectangleShape line;
};

