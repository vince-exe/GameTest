#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "windows_utils.h"
#include "MainMenuTextureManager.h"

class MenuConfirmationExit {
private:
	void handleButtonClicks(UiUtils::WindowsReturnValues& checker, sf::Event& event, bool& exitRequested);

	void setTextures();

	void setSprite();

	void draw();

private:
	sf::RenderWindow* m_Window;
	Entity m_backBtn, m_exitBtn, m_Text;

public:
	void init(sf::RenderWindow& window, UiUtils::WindowsReturnValues& checker);
};


