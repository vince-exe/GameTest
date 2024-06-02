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
#include "MainGameWindow.h"

#include "../GameServer/NetPacket.h"
#include "../GameServer/network_utilities.h"

class MainMenu {
private:
	bool loadMusicSound();

	void setMusicSound();

	void setTextures();

	void initSprites();

	void draw();

	void handleKeyBoard(sf::Event& event);

	void handleButtonClicks(sf::Event& event);

	void handleClientConnection(std::string nick, std::string ip, int port);

	void displayTextThread(Entity& entity, int seconds);

	void handleMatchmakingClient(const NetMessages& msg, std::string nickname);

	void listenForMatchmaking(std::string nickname);

	void displayTextFunc(Entity& entity);

	void undoMatchmaking();

	void exitMenu();

	void matchFound(std::string nickname);

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	std::shared_ptr<Music> backgroundMusicPtr;
	std::shared_ptr<Sound> notificationSound;

	Entity matchText, settingsText, settings2Text, undoMatchText, mainText, quitText;
	Entity menuMsgs[4];
	Entity* msgToDisplay;

	bool displayText;
	bool exitRequested;
	bool displayGameWindow;
	std::atomic<bool> inMatchmaking;

	std::shared_ptr<Client> client;
	std::string nickname;

public:
	MainMenu();

	bool init();
};
