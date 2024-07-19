#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "Client.h"

#include "SettingsManager.h"

#include "MenuConfirmationExit.h"
#include "OptionsMainMenu.h"
#include "NicknameMenu.h"
#include "IpPortMenu.h"
#include "MainGameWindow.h"
#include "TextureManager.h"
#include "AudioManager.h"

#include "../GameServer/NetPacket.h"
#include "../GameServer/network_utilities.h"

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
	void setMusicAndSound(SettingsManager& settingsManager, AudioManager& audioManager);

	void setTextures(TextureManager& textureManager);

	void initSprites();

	void draw();

	void handleKeyBoard(sf::Event& event, TextureManager& textureManager, AudioManager& audioManager);

	void handleButtonClicks(sf::Event& event, TextureManager& textureManager, FontManager& fontManager, SettingsManager& settingsManager, AudioManager& audioManager);

	void handleClientConnection(std::string nick, std::string ip, int port, AudioManager& audioManager);

	void displayTextThread(Entity& entity, int seconds);

	void handleMatchmakingClient(const NetPacket::NetMessages& msg, AudioManager& audioManager, std::string nickname);

	void listenForMatchmaking(std::string nickname);

	void displayTextFunc(Entity& entity);

	void undoMatchmaking();

	void exitMenu(TextureManager& textureManager, AudioManager& audioManager);

	void matchFound(std::string nick);

public:
	MainMenu();

	bool init(TextureManager& textureManager, FontManager& fontManager, SettingsManager& settingsManager, AudioManager& audioManager);
};
