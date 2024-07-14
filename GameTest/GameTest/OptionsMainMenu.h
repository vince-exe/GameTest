#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "Music.h"
#include "utils.h"
#include "TextureManager.h"
#include "SettingsManager.h"
#include "FontManager.h"
#include "AudioManager.h"

class OptionsMainMenu {
private:
	sf::RenderWindow* m_Window;

	const sf::Color m_defCheckpointColor = sf::Color(163, 163, 163);
	const sf::Color m_selectedCheckpointColor = sf::Color(58, 59, 60);

	sf::RectangleShape m_Slider;
	sf::RectangleShape m_checkPoints[10];

	sf::Text m_musicText;
	Entity m_backBtn;

	int m_oldVolumeIndex;

private:
	void draw();

	void handleMouseButtons(sf::Event& event, bool& requestExit, SettingsManager& settingsManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker);

	void checkVolumeLevel(SettingsManager& settingsManager, AudioManager& audioManager, sf::Vector2f& position);

	void initSprites(FontManager& fontManager, AudioManager& audioManager);

	void setTextures(TextureManager& textureManager);

public:
	void init(sf::RenderWindow& window, TextureManager& textureManager, SettingsManager& settingsManager, FontManager& fontManager, AudioManager& audioManager, SkyfallUtils::WindowsReturnValues& checker);
};