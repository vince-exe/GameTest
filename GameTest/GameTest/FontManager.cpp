#include "FontManager.h"

sf::Font FontManager::fredokaOne;

bool FontManager::init() {
    return fredokaOne.loadFromFile("assets/FredokaOne-Regular.ttf");
}
