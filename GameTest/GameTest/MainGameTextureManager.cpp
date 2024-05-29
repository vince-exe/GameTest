#include "MainGameTextureManager.h"

sf::Texture MainGameTextureManager::backGameOptions;

bool MainGameTextureManager::init() {
	return backGameOptions.loadFromFile("assets/Game/GameOptionsBackText.png");
}
