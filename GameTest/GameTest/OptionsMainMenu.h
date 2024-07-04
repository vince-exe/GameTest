#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "Music.h"
#include "windows_utils.h"
#include "MainMenuTextureManager.h"
#include "SettingsManager.h"

class OptionsMainMenu {
private:
	std::shared_ptr<sf::RenderWindow> m_Window;

	const sf::Color m_defCheckpointColor = sf::Color(163, 163, 163);
	const sf::Color m_selectedCheckpointColor = sf::Color(58, 59, 60);

	sf::RectangleShape m_Slider;
	sf::RectangleShape m_checkPoints[10];

	Entity m_volumeText;
	Entity m_backBtn;

	int m_oldVolumeIndex;

private:
	void draw();

	void handleMouseButtons(std::shared_ptr<Music> backgroundMusicPtr, sf::Event& event, bool& requestExit, UiUtils::WindowsReturnValues& checker);

	void checkVolumeLevel(std::shared_ptr<Music> backgroundMusicPtr, sf::Vector2f& position);

	void initSprites(std::shared_ptr<Music> backgroundMusicPtr);

	void setTextures();

public:
	void init(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<Music> backgroundMusicPtr, UiUtils::WindowsReturnValues& checker);
};