#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "Client.h"

#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"
#include "NicknameMenu.h"
#include "IpPortMenu.h"
#include "MainGameWindow.h"
#include "extern_variables.h"

#include "../GameServer/NetPacket.h"
#include "../GameServer/NetUdpPacket.h"

class MainMenu {
private:
	sf::RenderWindow m_Window;

	Entity m_matchText, m_settingsText, m_settings2Text, m_undoMatchText, m_mainText, m_quitText;
	Entity m_menuMsgs[4];
	Entity* m_msgToDisplay;

	bool m_displayText;
	bool m_exitRequested;
	bool m_displayGameWindow;
	std::atomic<bool> m_inMatchmaking;

	Client m_Client;
	std::string m_Nickname;

private:
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
