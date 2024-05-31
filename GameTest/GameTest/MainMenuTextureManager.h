#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class MainMenuTextureManager {
public:
	static bool init();

public:
	/* buttons */
	static sf::Texture backText, exitText, matchText, settingsText, doneText, connectText, cancelText, undoMatchText;
	/* text */
	static sf::Texture mainLobbyText, optionsTextVolume, confirmationExitText, nicknameText, connectionText, invalidFormatText;
	/* single volume text: 0 / 10 / 20... */
	static sf::Texture volumeTextLevel[10];
	/* menu texts / errors */
	static sf::Texture menuMsg[4];
};
