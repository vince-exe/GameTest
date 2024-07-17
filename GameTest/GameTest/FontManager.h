#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"

class FontManager {
private:
	FontManager() = default;
	~FontManager() = default;
	FontManager& operator=(const FontManager&) = delete; // Disable = operator
	FontManager& operator=(FontManager&&) = delete; // disable move operator

private:
	sf::Font m_fredokaOne;

public:
	static FontManager& getInstance();

public:	
	bool init();

	const sf::Font& getFredokaOne() const ;
};

