#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "Client.h"
#include "Player.h"
#include "../GameServer/network_utilities.h"
#include "network_game_utilities.h"
#include "Game.h"
#include "GameSettingsMenu.h"
#include "EndGameWindow.h"
#include "extern_variables.h"

class MainGameWindow {
private:
	sf::RenderWindow m_Window;
	Client* m_Client;
	Game m_Game;
	Game::GameSessionInfo m_sessionInfoStruct;

	sf::Text m_myNickname, m_enemyNickname, m_vsText, m_waitRoundText, m_gameTimer;

	float m_rechargeBarProgress;
	sf::RectangleShape m_rechargeBarBorder;
	sf::RectangleShape m_rechargeBar;

	sf::RectangleShape m_youHealth[3];
	sf::RectangleShape m_enemyHealth[3];

	Player m_youPlayer, m_enemyPlayer;

	bool m_displayWindow, m_inGameSettings, m_isCursorGrabbed;
	std::atomic<bool> m_closeSettingsWindowFlag;

	GameSettingsMenu m_gameSettingsMenu;
	EndGameWindow m_endGameWindow;

	std::vector<std::vector<sf::CircleShape>> m_damageAreasVector;

	std::unique_ptr<NetUdpPacket> m_udpPositionPacket;
	size_t m_positionPacketCounter, m_enemyPositionPacketCounter;

	Entity m_lockBtn, m_unlockBtn;

private:
	void setMusicAndSound();

	void update(sf::Time deltaTime);

	void draw();

	void getSessionInfo();

	void initSprites();

	void handleKeyBoards(sf::Event& event);

	void handleMouseClick(sf::Event& event);

	void quitGame();

	void updateRechargeBar();

	void checkPlayerWindowBorders(Player& player);

	bool handleEnemyNickname();

	void handleMessages();

	bool initPlayerAndEnemyPosition();

public:
	void init(const std::string nickname, Client& client);
};