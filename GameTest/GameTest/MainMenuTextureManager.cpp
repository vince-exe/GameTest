#include "MainMenuTextureManager.h"

sf::Texture MainMenuTextureManager::backBtn;
sf::Texture MainMenuTextureManager::exitBtn;
sf::Texture MainMenuTextureManager::playBtn;
sf::Texture MainMenuTextureManager::settingsBtn;
sf::Texture MainMenuTextureManager::doneBtn;
sf::Texture MainMenuTextureManager::background;
sf::Texture MainMenuTextureManager::optionsTextVolume;
sf::Texture MainMenuTextureManager::confirmationExitText;
sf::Texture MainMenuTextureManager::connectText;
sf::Texture MainMenuTextureManager::nicknameText;
sf::Texture MainMenuTextureManager::volumeTextLevel[10];
sf::Texture MainMenuTextureManager::menuMsg[4];
sf::Texture MainMenuTextureManager::cursorTextureGrab;
sf::Texture MainMenuTextureManager::cursorTexturePoint;
sf::Texture MainMenuTextureManager::connectBtn;
sf::Texture MainMenuTextureManager::cancelBtn;
sf::Texture MainMenuTextureManager::invalidFormatText;
sf::Texture MainMenuTextureManager::undoMatchBtn;

bool MainMenuTextureManager::init() {
    /* load the volume numbers */
    int pathNumber = 0;
    std::string pathStr;
    for (int i = 0; i < 10; i++) {
        pathStr = "assets/VolumeNumbers/" + std::to_string(pathNumber) + "_Text.png";
        if (!volumeTextLevel[i].loadFromFile(pathStr)) { return false; }
        pathNumber += 10;
    }
    pathNumber = 0;
    pathStr = "";
    /* load the menu texts */
    for (int i = 0; i < 4; i++) {
        pathStr = "assets/MenuTexts/" + std::to_string(pathNumber) + ".png";
        if (!menuMsg[i].loadFromFile(pathStr)) { return false; }
        pathNumber++;
    }

    return backBtn.loadFromFile("assets/Buttons/Back_Button.png") && exitBtn.loadFromFile("assets/Buttons/exitBtn.png")
        && playBtn.loadFromFile("assets/Buttons/playBtn.png") && settingsBtn.loadFromFile("assets/Buttons/settingsBtn.png")
        && doneBtn.loadFromFile("assets/Buttons/doneBtn.png") && background.loadFromFile("assets/Backgrounds/Background.png")
        && optionsTextVolume.loadFromFile("assets/VolumeNumbers/music_text.png") && confirmationExitText.loadFromFile("assets/Messages-Texts/img.png")
        && cursorTextureGrab.loadFromFile("assets/Cursor/Cursor_Grab.png") && cursorTexturePoint.loadFromFile("assets/Cursor/Cursor_Point.png")
        && nicknameText.loadFromFile("assets/Messages-Texts/nick_img.png") && connectBtn.loadFromFile("assets/Buttons/connectBtn.png")
        && cancelBtn.loadFromFile("assets/Buttons/cancelBtn.png") && connectText.loadFromFile("assets/Messages-Texts/connect_to_server.png")
        && invalidFormatText.loadFromFile("assets/Messages-Texts/invalid_format.png") && undoMatchBtn.loadFromFile("assets/Buttons/undoMatch.png");
}
