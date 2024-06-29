#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Entity.h"
#include "MainMenuTextureManager.h"

class EndGameWindow {
private:
	bool m_closeWindow;

	sf::Text m_resultsText, m_gameText, m_enemyQuitText;
	
	Entity m_doneButton;

private:
	void initTextures();

	void initSprites(sf::RenderWindow& window, Game& game);

	void draw(sf::RenderWindow& window, Game& game, sf::Text& playerNickText, sf::Text& vsText, sf::Text& enemyNickText);

	void handleMouseButtons(sf::Event& event, sf::RenderWindow& window);

public:
	void init(sf::RenderWindow& window, Game& game, sf::Text& playerNickText, sf::Text& vsText, sf::Text& enemyNickText);
};

