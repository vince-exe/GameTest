#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class MainGameTextureManager {
public:
	static bool init();

public:
	static sf::Texture background;
	/* checkpoint-buttons */
	static sf::Texture catCheckBtn, rabbitCheckBtn, foxCheckBtn, mouseCheckBtn;
	/* counters */
	static sf::Texture catCounter, foxCounter, mouseCounter, rabbitCounter;
};

