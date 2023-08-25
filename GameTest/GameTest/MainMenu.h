#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "Music.h"

#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"
#include "MainMenuTextureManager.h"
#include "NicknameMenu.h"

class MainMenu {
public:
	MainMenu();

	void setTextures();

	bool loadMouse();

	bool loadMusic();

	bool init();

private:
	void initSprites();

	void renderWindow();

	void handleMouseCursor(sf::Event& event);

	void handleButtonClicks(sf::Event& event, bool& exitRequested);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::shared_ptr<Music> backgroundMusicPtr;

	Entity background, playBtn, settingsBtn, exitBtn;

	sf::Cursor defaultCursor, pointCursor;
};
