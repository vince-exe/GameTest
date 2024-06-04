#pragma once

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "MainMenuTextureManager.h"

class GameSettingsMenu {
private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	Entity backText;

	bool exitWindow;
	std::atomic<bool>* closeFlag;

private:
	void setTextures();
	
	void setSprites();

	void draw();

	void handleMouseButtonPressed(sf::Event& event);

public:
	GameSettingsMenu() = default;

	bool init(std::shared_ptr<sf::RenderWindow> windowPtr, std::atomic<bool>* closeFlag);
};

