#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "ui_utils.h"
#include "MainMenuTextureManager.h"
#include "FontManager.h"

class NicknameMenu {
public:
	std::string init(std::shared_ptr<sf::RenderWindow> windowPtr, PopupReturnValues& checker);

private:
	void draw();

	void setTextures();

	void initSprites();

	void handleTextEntered(sf::Event& event);
	
	void handleMouseButtons(sf::Event& event, PopupReturnValues& checker, bool& exitRequested);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::string inputText;
	
	sf::Text inputDisplay;
	sf::RectangleShape line;

	Entity text;
	Entity doneBtn, backBtn;
};

