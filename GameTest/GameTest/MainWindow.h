#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "PopupExitWindow.h"

class MainWindow {
public:
	MainWindow();

	bool loadTextures();

	bool loadMouse();

	void init();

private:
	void initSprites();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	Entity background, playBtn, settingsBtn, exitBtn;

	sf::Texture cursorTextureGrab;
	sf::Texture cursorTexturePoint;

	sf::Cursor defaultCursor, pointCursor;
};
