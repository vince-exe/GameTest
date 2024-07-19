#include "FontManager.h"

bool FontManager::init() {
    return m_fredokaOne.loadFromFile(SkyfallUtils::Font::FREDOKA_ONE);
}

const sf::Font& FontManager::getFredokaOne() const {
    return m_fredokaOne;
}

FontManager& FontManager::getInstance() {
    static FontManager istance;
    return istance;
}
