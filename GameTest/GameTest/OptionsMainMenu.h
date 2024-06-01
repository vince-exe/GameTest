#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Entity.h"
#include "Music.h"
#include "ui_utils.h"
#include "MainMenuTextureManager.h"
#include "SettingsManager.h"

class OptionsMainMenu {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, std::shared_ptr<Music> backgroundMusicPtr, PopupReturnValues& checker);

private:
	void draw();

	void handleMouseButtons(std::shared_ptr<Music> backgroundMusicPtr, sf::Event& event, bool& requestExit, PopupReturnValues& checker);

	void checkVolumeLevel(std::shared_ptr<Music> backgroundMusicPtr, sf::Vector2f& position);

	void initSprites(std::shared_ptr<Music> backgroundMusicPtr);

	void setTextures();

private:
	std::shared_ptr<sf::RenderWindow> windowPtr;

	const sf::Color defCheckpointColor = sf::Color(163, 163, 163);
	const sf::Color selectedCheckpointColor = sf::Color(58, 59, 60);

	sf::RectangleShape slider;
	sf::RectangleShape checkPoints[10];

	Entity volumeText;
	Entity backBtn;

	int oldVolumeIndex;
};

