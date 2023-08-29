#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

class MainMenuTextureManager {
public:
	static bool init();

public:
	/* buttons */
	static sf::Texture backBtn, exitBtn, playBtn, settingsBtn, doneBtn;
	static sf::Texture background;
	/* text */
	static sf::Texture optionsTextVolume, confirmationExitText, nicknameText;
	/* single volume text: 0 / 10 / 20... */
	static sf::Texture volumeTextLevel[10];
	/* mouse */
	static sf::Texture cursorTextureGrab, cursorTexturePoint;
	/* menu texts / errors */
	static sf::Texture menuMsg[4];
};
