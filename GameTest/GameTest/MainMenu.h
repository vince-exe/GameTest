#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"

class MainMenu {
public:
	MainMenu();

	bool loadTextures();

	bool loadMouse();

	bool init();

private:
	void initSprites();

	void renderWindow();

	void handleMouseCursor(sf::Event& event);

	void handleButtonClicks(sf::Event& event, bool& exitRequested);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	Entity background, playBtn, settingsBtn, exitBtn;

	sf::Texture cursorTextureGrab;
	sf::Texture cursorTexturePoint;

	sf::Cursor defaultCursor, pointCursor;
};
