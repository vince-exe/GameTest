#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Game.h"

class EndGameWindow {
private:
	bool m_closeWindow;
	sf::Text m_resultsText;

private:
	void initSprites(sf::RenderWindow& window, Game& game);

	void draw(sf::RenderWindow& window);

public:
	void init(sf::RenderWindow& window, Game& game);
};

