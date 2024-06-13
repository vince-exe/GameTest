#include "TemporaryThread.h"

int TemporaryThread::uselessThreadCounter = 0;

TemporaryThread::TemporaryThread(std::shared_ptr<std::thread> t, bool cancellable) {
	this->t = t;
	this->cancellable = cancellable;
}

std::shared_ptr<std::thread> TemporaryThread::getThread() {
	return this->t;
}

bool TemporaryThread::isCancellable() {
	return this->cancellable;
}

void TemporaryThread::setCancellable() {
	this->cancellable = true;
}