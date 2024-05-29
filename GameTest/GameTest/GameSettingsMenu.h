#pragma once

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "MainGameTextureManager.h"

class GameSettingsMenu {
private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	Entity backText;

private:
	void setTextures();

	void setSprites();

	void draw();

public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr);
};

