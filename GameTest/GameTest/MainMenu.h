#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "Music.h"
#include "Client.h"
#include "Sound.h"

#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"
#include "MainMenuTextureManager.h"
#include "NicknameMenu.h"


#include "../GameServer/NetPacket.h"
#include "../GameServer/network_utilities.h"

class MainMenu {
public:
	MainMenu();

	bool init();

private:
	bool loadMouse();

	bool loadMusicSound();

	void setMusicSound();

	void setTextures();

	void initSprites();

	void renderWindow();

	void handleMouseCursor(sf::Event& event);

	void handleKeyBoard(sf::Event& event, bool& exitRequested);

	void handleButtonClicks(sf::Event& event, bool& exitRequested);

	void handleClientConnection(std::string nick);

	void displayTextFuncTime(Entity& entity, int seconds);

	bool handleConnectionMsg(const NetMessages& msg);

	void displayTextFunc(Entity& entity);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	std::shared_ptr<Music> backgroundMusicPtr;
	std::shared_ptr<Sound> notificationSound;

	Entity background, playBtn, settingsBtn, exitBtn;
	Entity menuMsgs[4];
	Entity* msgToDisplay;

	sf::Cursor defaultCursor, pointCursor;

	bool displayText;
};
