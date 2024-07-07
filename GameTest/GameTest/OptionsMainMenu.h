#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "Music.h"
#include "utils.h"
#include "TextureManager.h"
#include "SettingsManager.h"

class OptionsMainMenu {
private:
	sf::RenderWindow* m_Window;

	const sf::Color m_defCheckpointColor = sf::Color(163, 163, 163);
	const sf::Color m_selectedCheckpointColor = sf::Color(58, 59, 60);

	sf::RectangleShape m_Slider;
	sf::RectangleShape m_checkPoints[10];

	Entity m_volumeText;
	Entity m_backBtn;

	int m_oldVolumeIndex;

private:
	void draw();

	void handleMouseButtons(Music& backgroundMusic, sf::Event& event, bool& requestExit, SkyfallUtils::WindowsReturnValues& checker);

	void checkVolumeLevel(Music& backgroundMusic, sf::Vector2f& position);

	void initSprites(Music& backgroundMusic);

	void setTextures(TextureManager& textureManager);

public:
	void init(sf::RenderWindow& window, Music& backgroundMusic, TextureManager& textureManager, SkyfallUtils::WindowsReturnValues& checker);
};