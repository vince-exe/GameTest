#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "ui_utils.h"

class NicknameMenu {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupReturnValues& checker);

private:
	void draw(Entity& background);

	bool loadTextures();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	Entity doneBtn, backBtn;
};

