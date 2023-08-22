#include "MainMenu.h"
#include "MainMenuTextureManager.h"

int main() {
    if (!MainMenuTextureManager::init()) {
        std::cout << "\n[ ERROR ]: Failed to load some / all MainMenu textures\n";
        return 1;
    }
    
    MainMenu mainMenu;
    return mainMenu.init();
    
}