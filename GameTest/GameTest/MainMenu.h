#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "Music.h"
#include "Client.h"
#include "Sound.h"

#include "SettingsManager.h"

#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"
#include "MainMenuTextureManager.h"
#include "NicknameMenu.h"
#include "IpPortMenu.h"

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

	void handleKeyBoard(sf::Event& event);

	void handleButtonClicks(sf::Event& event);

	void handleClientConnection(std::string nick, std::string ip, int port);

	void displayTextFuncTime(Entity& entity, int seconds);

	void handleMatchmakingResponse(const NetMessages& msg);

	void listenForMatchmaking();

	void displayTextFunc(Entity& entity);

	void undoMatchmaking();

	void exitMenu();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	std::shared_ptr<Music> backgroundMusicPtr;
	std::shared_ptr<Sound> notificationSound;

	Entity background, playBtn, settingsBtn, exitBtn, undoMatchBtn;
	Entity menuMsgs[4];
	Entity* msgToDisplay;

	sf::Cursor defaultCursor, pointCursor;

	bool displayText;
	bool exitRequested;
	std::atomic<bool> inMatchmaking;

	std::shared_ptr<Client> client;
};
