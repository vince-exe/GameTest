#include "MainMenuTextureManager.h"

sf::Texture MainMenuTextureManager::backBtn;
sf::Texture MainMenuTextureManager::exitBtn;
sf::Texture MainMenuTextureManager::playBtn;
sf::Texture MainMenuTextureManager::settingsBtn;
sf::Texture MainMenuTextureManager::doneBtn;
sf::Texture MainMenuTextureManager::background;
sf::Texture MainMenuTextureManager::optionsTextVolume;
sf::Texture MainMenuTextureManager::confirmationExitText;
sf::Texture MainMenuTextureManager::volumeTextLevel[10];
sf::Texture MainMenuTextureManager::cursorTextureGrab;
sf::Texture MainMenuTextureManager::cursorTexturePoint;

bool MainMenuTextureManager::init() {
    /* load the volume numbers */
    int pathNumber = 0;
    std::string pathStr;
    for (int i = 0; i < 10; i++) {
        pathStr = "assets/" + std::to_string(pathNumber) + "_Text.png";
        if (!volumeTextLevel[i].loadFromFile(pathStr)) { return false; }
        pathNumber += 10;
    }
    
    return backBtn.loadFromFile("assets/Back_Button.png") && exitBtn.loadFromFile("assets/exitBtn.png")
        && playBtn.loadFromFile("assets/playBtn.png") && settingsBtn.loadFromFile("assets/settingsBtn.png")
        && doneBtn.loadFromFile("assets/doneBtn.png") && background.loadFromFile("assets/Background.png")
        && optionsTextVolume.loadFromFile("assets/Volume_Png.png") && confirmationExitText.loadFromFile("assets/img.png")
        && cursorTextureGrab.loadFromFile("assets/Cursor_Grab.png") && cursorTexturePoint.loadFromFile("assets/Cursor_Point.png");
}
