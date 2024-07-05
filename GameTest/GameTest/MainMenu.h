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
	sf::RenderWindow m_Window;
	Music m_backgroundMusic;
	Sound m_notificationSound;

	Entity m_matchText, m_settingsText, m_settings2Text, m_undoMatchText, m_mainText, m_quitText;
	Entity m_menuMsgs[4];
	Entity* m_msgToDisplay;

	bool m_displayText;
	bool m_exitRequested;
	bool m_displayGameWindow;
	std::atomic<bool> m_inMatchmaking;

	std::shared_ptr<Client> m_Client;
	std::string m_Nickname;

private:
	bool loadMusicSound();

	void setMusicAndSound();

	void setTextures();

	void initSprites();

	void draw();

	void handleKeyBoard(sf::Event& event);

	void handleButtonClicks(sf::Event& event);

	void handleClientConnection(std::string nick, std::string ip, int port);

	void displayTextThread(Entity& entity, int seconds);

	void handleMatchmakingClient(const NetPacket::NetMessages& msg, std::string nickname);

	void listenForMatchmaking(std::string nickname);

	void displayTextFunc(Entity& entity);

	void undoMatchmaking();

	void exitMenu();

	void matchFound(std::string nick);

public:
	MainMenu();

	bool init();
};
