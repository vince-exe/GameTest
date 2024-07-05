#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <thread>

#include  "SettingsManager.h"

#include "Entity.h"
#include "windows_utils.h"
#include "MainMenuTextureManager.h"
#include "FontManager.h"
#include "Sound.h"

class IpPortMenu {
private:
	sf::RenderWindow* m_Window;
	std::pair<std::string, int> m_Pair;

	sf::Text m_inputDisplay;
	sf::RectangleShape m_Line;

	Entity m_Text, m_connectBtn, m_cancelBtn, m_entityToDisplay;

	bool m_displayText;
	Entity* m_msgToDisplay;

private:
	void setTextures();

	void setPlaceholder();

	void initSprites();

	void draw();

	void handleTextEntered(sf::Event& event);

	void handleMouseButtons(sf::Event& event, UiUtils::WindowsReturnValues& checker, bool& exitRequested, Sound& notificationSound);

	bool setIpPort(std::string ipPort);

	void displayTextFuncTime(Entity& entity, int seconds);

public:
	std::pair<std::string, int> init(sf::RenderWindow& window, Sound& notificationSound, UiUtils::WindowsReturnValues& checker);
};

