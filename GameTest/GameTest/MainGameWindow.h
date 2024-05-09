#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

#include "Entity.h"
#include "FontManager.h"
#include "Client.h"
#include "../GameServer/network_utilities.h"

#include "MainGameTextureManager.h"

class MainGameWindow {
public:
	void init(const std::string nickname, std::shared_ptr<Client> client);

private:
	void draw();

	void setTextures();

	void initSprites();

	bool handleEnemyNickname();

	void handlePlayerMovement(sf::Event& event);

	void quitGame();

	void handleMessages();

	bool initPlayerAndEnemyPosition();

private:

	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::shared_ptr<Client> client;

	sf::Text myNickname, enemyNickname;

	/* temp for testing */
	sf::RectangleShape youPlayer, enemyPlayer;
};

