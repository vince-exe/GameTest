#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "ui_utils.h"
#include "MainMenuTextureManager.h"

class MenuConfirmationExit {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupReturnValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor);
	
private:
	void handleMouseCursor(sf::Cursor& pointCursor, sf::Cursor& defCursor);

	void handleButtonClicks(PopupReturnValues& checker, bool& exitRequested);

	void setTextures();

	void setSprite();

	void renderWindow(Entity& background);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	Entity backBtn, exitBtn, text;
};


