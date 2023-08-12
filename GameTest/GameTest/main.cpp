#include "MainWindow.h"

int main() {
    MainWindow mainWindow;

    if (!mainWindow.initTextures()) {
        std::cout << "\n[ Error ]: Failed to load same / all Game's textures";
    }
    mainWindow.init();
    return 0;
}