#include "NicknameMenu.h"

void NicknameMenu::init(std::shared_ptr<sf::RenderWindow> windowPtr, Entity& background, PopupReturnValues& checker) {
	this->windowPtr = windowPtr;
}

void NicknameMenu::draw(Entity& background) {
	windowPtr->clear();
	windowPtr->draw(background);
	windowPtr->display();
}

bool NicknameMenu::loadTextures() {
	return false;
}
