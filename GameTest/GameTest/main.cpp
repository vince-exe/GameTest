#include "MainMenu.h"
#include "MainMenuTextureManager.h"
#include "FontManager.h"

int main() {
    if (!MainMenuTextureManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load some / all MainMenu textures\n";
        return 1;
    }
    if (!FontManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load game's fonts\n";
        return 1;
    }

    MainMenu mainMenu;
    return mainMenu.init();
    
}