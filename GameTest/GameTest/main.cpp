#include "MainMenu.h"
#include "TextureManager.h"
#include "FontManager.h"
#include "SettingsManager.h"
#include <iostream>

int main() {
    if (!SettingsManager::init()) {
        std::cout << "\n[ ERROR ]: Settings init method failed";
        return 1;
    }
    if (!FontManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load game's fonts";
        return 1;
    }
    TextureManager& singleton = TextureManager::getInstance();
    if (!singleton.init()) {
        std::cerr << "\n[ ERROR ]: Failed to load game's textures";
        return 1;
    }

    MainMenu mainMenu;
    mainMenu.init(singleton);
    
    if (SettingsManager::storeSettings()) {
        std::cout << "\nSuccessfully stored the settings";
    }
    else {
        std::cout << "\nFailed to store the settings";
    }
}