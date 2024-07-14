#include <iostream>

#include "MainMenu.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "SettingsManager.h"
#include "AudioManager.h"

int main() {
    SettingsManager& sSingleton = SettingsManager::getInstance();
    FontManager& fSinfleton = FontManager::getInstance();
    TextureManager& tSingleton = TextureManager::getInstance();
    AudioManager& aSingleton = AudioManager::getInstance();

    if (!sSingleton.init(SkyfallUtils::Settings::SETTINGS_PATH)) {
        std::cerr << "\n[ ERROR ]: Failed to load game's settings";
        return 1;
    }
    if (!fSinfleton.init()) {
        std::cerr << "\n [ ERROR ]: Failed to load game's fonts";
        return 1;
    }
    if (!tSingleton.init()) {
        std::cerr << "\n[ ERROR ]: Failed to load game's textures";
        return 1;
    }
    if (!aSingleton.init()) {
        std::cerr << "\n[ ERROR ]: Failed to load game's audio";
        return 1;
    }

    MainMenu mainMenu;
    mainMenu.init(tSingleton, fSinfleton, sSingleton, aSingleton);
    
    if (sSingleton.storeSettings()) {
        std::cout << "\nSuccessfully stored the settings";
    }
    else {
        std::cout << "\nFailed to store the settings";
    }

    return 0;
}