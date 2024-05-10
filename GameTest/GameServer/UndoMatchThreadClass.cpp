#include "UndoMatchThreadClass.h"

int UndoMatchThreadClass::uselessThreadCounter = 0;

UndoMatchThreadClass::UndoMatchThreadClass(std::shared_ptr<std::thread> t, bool cancellable) {
	this->t = t;
	this->cancellable = cancellable;
}

std::shared_ptr<std::thread> UndoMatchThreadClass::getThread() {
	return this->t;
}

bool UndoMatchThreadClass::isCancellable() {
	return this->cancellable;
}

void UndoMatchThreadClass::setCancellable() {
	this->cancellable = true;
}
