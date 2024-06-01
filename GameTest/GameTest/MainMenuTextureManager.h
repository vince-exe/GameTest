#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class MainMenuTextureManager {
public:
	static bool init();

public:
	/* buttons */
	static sf::Texture quitText, matchText, settingsText, doneText, connectText, cancelText, undoMatchText;
	/* text */
	static sf::Texture mainLobbyText, optionsTextVolume, confirmationExitText, nicknameText, connectionText, invalidFormatText;
	/* menu texts / errors */
	static sf::Texture menuMsg[4];
};
