#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

class MainWindow {
public:
	MainWindow();

	bool initTextures();

	void init();

private:
	void initSprites();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	
};
