#pragma once

#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"
#include "Player.h"
#include "extern_variables.h"

class GameSettingsMenu {
private:
	Entity m_backText;

	sf::Text m_debugModeText, m_debugModeResult, m_musicText, m_soundEffectsText;
	int m_oldMusicVolumeIndex, m_oldSoundVolumeIndex;

	const sf::Color m_defCheckpointColor = sf::Color(163, 163, 163);
	const sf::Color m_selectedCheckpointColor = sf::Color(58, 59, 60);

	sf::RectangleShape m_musicSlider, m_soundEffectsSlider;
	sf::RectangleShape m_musicCheckPoints[10], m_soundEffectsCheckpoints[10];

public:
	GameSettingsMenu() = default;

	void setTextures();

	void setSprites(sf::RenderWindow& window);

	void draw(sf::RenderWindow& window);

	void setMusicLevel(sf::RenderWindow& window, sf::Vector2f& position);

	void setSoundLevel(sf::RenderWindow& window, sf::Vector2f& position);

	void handleMouseButtonPressed(sf::Event& event, sf::RenderWindow& window, bool& inGameSettings, Player& player);

	void init(sf::RenderWindow& window);
};