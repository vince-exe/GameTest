#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "windows_utils.h"
#include "MainMenuTextureManager.h"
#include "FontManager.h"

class NicknameMenu {
private:
	std::shared_ptr<sf::RenderWindow> m_Window;
	std::string m_inputText;

	sf::Text m_inputDisplay;
	sf::RectangleShape m_Line;

	Entity m_Text;
	Entity m_doneBtn, m_cancelBtn;

public:
	std::string init(std::shared_ptr<sf::RenderWindow> window, UiUtils::WindowsReturnValues& checker);

private:
	void draw();

	void setTextures();

	void initSprites();

	void handleTextEntered(sf::Event& event);
	
	void handleMouseButtons(sf::Event& event, UiUtils::WindowsReturnValues& checker, bool& exitRequested);
};

