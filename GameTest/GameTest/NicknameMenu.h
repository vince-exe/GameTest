#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "utils.h"
#include "TextureManager.h"
#include "FontManager.h"

class NicknameMenu {
private:
	sf::RenderWindow* m_Window;
	std::string m_inputText;

	sf::Text m_inputDisplay;
	sf::RectangleShape m_Line;

	Entity m_Text;
	Entity m_doneBtn, m_cancelBtn;

public:
	std::string init(sf::RenderWindow& window, TextureManager& textureManager, FontManager& fontManager, SkyfallUtils::WindowsReturnValues& checker);

private:
	void draw();

	void setTextures(TextureManager& textureManager);

	void initSprites(FontManager& fontManager);

	void handleTextEntered(sf::Event& event);
	
	void handleMouseButtons(sf::Event& event, SkyfallUtils::WindowsReturnValues& checker, bool& exitRequested);
};

