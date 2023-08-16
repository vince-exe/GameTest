#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"

enum PopupExitWindowValues {
	TEXTURE_FAIL,
	EXIT
};

class PopupExitWindow {
public:
	void init(Entity& background, PopupExitWindowValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor);

	bool loadTextures();

private:
	sf::RenderWindow window;

	Entity backBtn, exitBtn, text;
};


