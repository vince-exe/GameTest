#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "FontManager.h"
#include "Client.h"
#include "Player.h"
#include "../GameServer/network_utilities.h"
#include "MainGameTextureManager.h"
#include "network_game_utilities.h"
#include "Game.h"

#include "GameSettingsMenu.h"
#include "EndGameWindow.h"

class MainGameWindow {
private:
	void update(sf::Time deltaTime);

	void draw();

	void initSprites();
	
	void handleKeyBoards(sf::Event event);

	void handleMouseClick(sf::Event& event);
	
	void quitGame();

	void updateRechargeBar();

	void checkPlayerWindowBorders();

	bool handleEnemyNickname();

	void handleMessages();
	
	bool initPlayerAndEnemyPosition();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::shared_ptr<Client> client;
	Game m_Game;

	sf::Text myNickname, enemyNickname, vsText, m_waitRoundText, m_gameTimer;

	float rechargeBarProgress;
	sf::RectangleShape rechargeBarBorder;
	sf::RectangleShape rechargeBar;

	sf::RectangleShape youHealth[3];
	sf::RectangleShape enemyHealth[3];

	std::shared_ptr<Player> youPlayer, enemyPlayer;

	bool displayWindow, inGameSettings;
	std::atomic<bool> closeSettingsWindowFlag;

	GameSettingsMenu gameSettingsMenu;
	EndGameWindow endGameWindow;

	std::vector<std::vector<sf::CircleShape>> m_damageAreasVector;

public:
	void init(const std::string nickname, std::shared_ptr<Client> client);
};

