#pragma once

#include <SFML/Graphics.hpp>

#include "utils.h"

class FontManager {
public:
	FontManager() = default;
	~FontManager() = default;
	FontManager& operator=(const FontManager&) = delete; // Disable = operator
	FontManager& operator=(FontManager&&) = delete; // disable move operator

private:
	sf::Font m_fredokaOne;

public:	
	bool init();

	const sf::Font& getFredokaOne() const ;
};