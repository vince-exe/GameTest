#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

class MainWindow {
public:
	MainWindow();

	bool loadTextures();

	bool loadMouse();

	void init();

private:
	void initSprites();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	
	sf::Texture playBtnText;
	sf::Sprite playBtnSprite;

	sf::Texture settingBtnText;
	sf::Sprite settingBtnSprite;

	sf::Texture exitBtnText;
	sf::Sprite exitBtnSprite;

	sf::Texture cursorTextureGrab;
	sf::Cursor customCursor;
};
