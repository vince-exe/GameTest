#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "Music.h"

#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"
#include "MainMenuTextureManager.h"
#include "NicknameMenu.h"
#include "Client.h"

#include "../GameServer/NetPacket.h"
#include "../GameServer/network_utilities.h"

class MainMenu {
public:
	MainMenu();

	bool init();

	void handleClientConnection(std::string nick);

private:
	bool loadMouse();

	bool loadMusic();

	void setTextures();

	void initSprites();

	void renderWindow();

	void handleMouseCursor(sf::Event& event);

	void handleButtonClicks(sf::Event& event, bool& exitRequested);

	bool handleConnectionMsg(const NetMessages& msg);

	void displayTextFunc(Entity& entity);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::shared_ptr<Music> backgroundMusicPtr;

	Entity background, playBtn, settingsBtn, exitBtn;
	Entity menuMsgs[4];
	Entity* msgToDisplay;

	sf::Cursor defaultCursor, pointCursor;

	bool displayText;
};
