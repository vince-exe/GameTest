#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Entity.h"

enum PopupExitWindowValues {
	TEXTURE_FAIL,
	EXIT,
	BACK
};

class PopupExitWindow {
public:
	void init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupExitWindowValues& checker, sf::Cursor& defCursor, sf::Cursor& pointCursor);

	bool loadTextures();

private:
	Entity backBtn, exitBtn, text;
};


