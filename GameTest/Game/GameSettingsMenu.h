#pragma once

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Player.h"
#include "extern_variables.h"

class GameSettingsMenu {
private:
	Entity m_backText;
	sf::Text m_debugModeText, m_debugModeResult;

public:
	GameSettingsMenu() = default;

	void setTextures();

	void setSprites(sf::RenderWindow& window);

	void draw(sf::RenderWindow& window);

	void handleMouseButtonPressed(sf::Event& event, sf::RenderWindow& window, bool& inGameSettings, Player& player);

	void init(sf::RenderWindow& window);
};