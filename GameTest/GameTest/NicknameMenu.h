#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "ui_utils.h"
#include "MainMenuTextureManager.h"
#include "FontManager.h"

class NicknameMenu {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupReturnValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor);

private:
	void draw(Entity& background);

	void setTextures();

	void initSprites();

	void handleTextEntered(sf::Event& event);

	void handleMouseCursor(sf::Cursor& pointCursor, sf::Cursor& defCursor);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::string inputText;
	
	sf::Text inputDisplay;
	sf::RectangleShape line;

	Entity text;
	Entity doneBtn, backBtn;
};

