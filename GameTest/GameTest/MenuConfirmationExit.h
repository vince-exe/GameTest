#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "utils.h"
#include "TextureManager.h"

class MenuConfirmationExit {
private:
	void handleButtonClicks(SkyfallUtils::WindowsReturnValues& checker, sf::Event& event, bool& exitRequested);

	void setTextures(TextureManager& textureManager);

	void setSprite();

	void draw();

private:
	sf::RenderWindow* m_Window;
	Entity m_backBtn, m_exitBtn, m_Text;

public:
	void init(sf::RenderWindow& window, TextureManager& textureManager, SkyfallUtils::WindowsReturnValues& checker);
};


