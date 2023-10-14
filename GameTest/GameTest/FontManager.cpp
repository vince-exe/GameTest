#include "FontManager.h"

sf::Font FontManager::fredokaOne;

bool FontManager::init() {
    return fredokaOne.loadFromFile("assets/Fonts/FredokaOne-Regular.ttf");
}
