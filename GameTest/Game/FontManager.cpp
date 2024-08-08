#include "FontManager.h"

bool FontManager::init() {
    return m_fredokaOne.loadFromFile(SkyfallUtils::Font::FREDOKA_ONE.string());
}

const sf::Font& FontManager::getFredokaOne() const {
    return m_fredokaOne;
}