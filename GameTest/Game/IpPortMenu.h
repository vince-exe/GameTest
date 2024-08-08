#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include <thread>

#include "Entity.h"
#include "utils.h"
#include "extern_variables.h"

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

	void handleMouseButtons(sf::Event& event, SkyfallUtils::WindowsReturnValues& checker, bool& exitRequested);

	bool setIpPort(std::string ipPort);

	void displayTextFuncTime(Entity& entity, int seconds);

public:
	std::pair<std::string, int> init(sf::RenderWindow& window, SkyfallUtils::WindowsReturnValues& checker);
};

