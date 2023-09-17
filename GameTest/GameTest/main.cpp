#include "MainMenu.h"
#include "MainMenuTextureManager.h"
#include "FontManager.h"
#include "SettingsManager.h"

int main() {
    if (!SettingsManager::init()) {
        std::cout << "\n[ ERROR ]: Settings init method failed.\n";
        return 1;
    }
    if (!MainMenuTextureManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load some / all MainMenu textures\n";
        return 1;
    }
    if (!FontManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load game's fonts\n";
        return 1;
    }

    MainMenu mainMenu;
    mainMenu.init();
    
    if (SettingsManager::storeSettings()) {
        std::cout << "\nSuccessfully stored the settings\n";
    }
    else {
        std::cout << "\nFailed to store the settings\n";
    }
}