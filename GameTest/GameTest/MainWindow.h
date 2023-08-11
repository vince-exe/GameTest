#pragma once

#include <SFML/Window.hpp>
#include <iostream>

class MainWindow {
public:
	MainWindow();

	void init();

private:
	std::shared_ptr<sf::Window> windowPtr;
};
