#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "FontManager.h"
#include "Client.h"
#include "../GameServer/network_utilities.h"

#include "MainGameTextureManager.h"

class MainGameWindow {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr_, const std::string nickname, std::shared_ptr<Client> client);

private:
	void draw();

	void setTextures();

	void initSprites();

	void handleEnemyNickname();

private:

	std::shared_ptr<sf::RenderWindow> windowPtr;
	std::shared_ptr<Client> client;

	sf::Text myNickname, enemyNickname;
};

