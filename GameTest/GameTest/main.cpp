#include "MainMenu.h"
#include "MainMenuTextureManager.h"
#include "MainGameTextureManager.h"
#include "FontManager.h"
#include "SettingsManager.h"

/* temp include */
#include "MainGameWindow.h"
#include "../GameServer/network_utilities.h"

#include <iostream>

int main() {
    if (!SettingsManager::init()) {
        std::cout << "\n[ ERROR ]: Settings init method failed";
        return 1;
    }
    if (!MainMenuTextureManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load some / all MainMenu textures";
        return 1;
    }
    if (!MainGameTextureManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load some / all Game textures";
        return 1;
    }
    if (!FontManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load game's fonts";
        return 1;
    }
    
    MainGameWindow mainGameWindow;
    mainGameWindow.init(nullptr, "VINZ", nullptr);
    
    /* TEMP 
    MainMenu mainMenu;
    mainMenu.init();
    */

    if (SettingsManager::storeSettings()) {
        std::cout << "\nSuccessfully stored the settings";
    }
    else {
        std::cout << "\nFailed to store the settings";
    }
}