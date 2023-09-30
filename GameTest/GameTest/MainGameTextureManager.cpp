#include "MainGameTextureManager.h"

sf::Texture MainGameTextureManager::background;

bool MainGameTextureManager::init() {
    return background.loadFromFile("assets/gameBackground.png");
}
