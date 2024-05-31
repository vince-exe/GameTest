#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "ui_utils.h"
#include "MainMenuTextureManager.h"

class MenuConfirmationExit {
private:
	void handleButtonClicks(PopupReturnValues& checker, sf::Event& event, bool& exitRequested);

	void setTextures();

	void setSprite();

	void draw();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	Entity backBtn, exitBtn, text;

public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, PopupReturnValues& checker);
};


