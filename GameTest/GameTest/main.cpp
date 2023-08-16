#include "MainWindow.h"

int main() {
    MainWindow mainWindow;

    if (!mainWindow.loadTextures()) {
        std::cout << "\n[ Error ]: Failed to load some / all Game's textures ( MainWindow )";
        return -1;
    }
    if (!mainWindow.loadMouse()) {
        std::cout << "\n[ Error ]: Failed to load the cursor..";
        return -1;
    }

    mainWindow.init();
    return 0;
}