#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace SkyfallUtils {
	/*
		the 3 value a window can return 
	*/
	enum class WindowsReturnValues {
		DONE,
		EXIT,
		BACK
	};

	bool doLinesIntersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4);

	bool doesRectangleIntersectLine(const sf::RectangleShape& rectangle, const sf::VertexArray& line);
}