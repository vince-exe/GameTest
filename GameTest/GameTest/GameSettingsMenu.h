#pragma once

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "MainMenuTextureManager.h"

class GameSettingsMenu {
private:
	sf::RenderWindow* m_Window;

	Entity m_backText;

	bool m_exitWindow;
	std::atomic<bool>* m_closeFlag;

private:
	void setTextures();
	
	void setSprites();

	void draw();

	void handleMouseButtonPressed(sf::Event& event);

public:
	GameSettingsMenu() = default;

	bool init(sf::RenderWindow& window, std::atomic<bool>* closeFlag);
};

