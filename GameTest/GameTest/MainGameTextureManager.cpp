#include "MainGameTextureManager.h"

sf::Texture MainGameTextureManager::background;
sf::Texture MainGameTextureManager::catCheckBtn;
sf::Texture MainGameTextureManager::rabbitCheckBtn;
sf::Texture MainGameTextureManager::foxCheckBtn;
sf::Texture MainGameTextureManager::mouseCheckBtn;
sf::Texture MainGameTextureManager::catCounter;
sf::Texture MainGameTextureManager::foxCounter;
sf::Texture MainGameTextureManager::mouseCounter;
sf::Texture MainGameTextureManager::rabbitCounter;

bool MainGameTextureManager::init() {
    return background.loadFromFile("assets/Backgrounds/gameBackground.png") && catCheckBtn.loadFromFile("assets/Game/Checkpoint-Buttons/cat.png")
        && rabbitCheckBtn.loadFromFile("assets/Game/Checkpoint-Buttons/rabbit.png") && foxCheckBtn.loadFromFile("assets/Game/Checkpoint-Buttons/fox.png")
        && mouseCheckBtn.loadFromFile("assets/Game/Checkpoint-Buttons/mouse.png") && catCounter.loadFromFile("assets/Game/Counters/cat.png")
        && foxCounter.loadFromFile("assets/Game/Counters/fox.png") && mouseCounter.loadFromFile("assets/Game/Counters/mouse.png")
        && rabbitCounter.loadFromFile("assets/Game/Counters/rabbit.png");
}
