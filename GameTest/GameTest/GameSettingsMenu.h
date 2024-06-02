#pragma once

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "MainMenuTextureManager.h"

class GameSettingsMenu {
private:
	Entity backText;

	bool exitWindow;
	std::atomic<bool>* closeFlag;

private:
	void setTextures();
	
	void setSprites(std::shared_ptr<sf::RenderWindow> windowPtr);

	void draw(std::shared_ptr<sf::RenderWindow> windowPtr);

	void handleMouseButtonPressed(sf::Event& event, std::shared_ptr<sf::RenderWindow> windowPtr);

public:
	GameSettingsMenu() = default;

	bool init(std::shared_ptr<sf::RenderWindow> windowPtr, std::atomic<bool>* closeFlag);
};

