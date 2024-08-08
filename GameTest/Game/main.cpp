#include <iostream>

#include "extern_variables.h"
#include "MainMenu.h"

int main() {

    if (!g_sSingleton.init(SkyfallUtils::Settings::SETTINGS_PATH)) {
        std::cerr << "\n[ ERROR ]: Failed to load game's settings\n";
        system("pause");
        return 1;
    }
    if (!g_fSingleton.init()) {
        std::cerr << "\n [ ERROR ]: Failed to load game's fonts\n";
        system("pause");
        return 1;
    }
    if (!g_tSingleton.init()) {
        std::cerr << "\n[ ERROR ]: Failed to load game's textures\n";
        system("pause");
        return 1;
    }
    if (!g_aSingleton.init()) {
        std::cerr << "\n[ ERROR ]: Failed to load game's audio\n";
        system("pause");
        return 1;
    }

    MainMenu mainMenu;
    mainMenu.init();
    
    if (g_sSingleton.storeSettings()) {
        std::cout << "\nSuccessfully stored the settings";
    }
    else {
        std::cout << "\nFailed to store the settings";
    }
    
    return 0;
}