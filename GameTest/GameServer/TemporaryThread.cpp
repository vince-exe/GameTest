#include "TemporaryThread.h"

TemporaryThread::TemporaryThread(std::shared_ptr<std::thread> t, bool cancellable) {
	m_Thread = t;
	m_Cancellable = cancellable;
}

std::shared_ptr<std::thread> TemporaryThread::getThread() const {
	return m_Thread;
}

bool TemporaryThread::isCancellable() const {
	return m_Cancellable;
}

void TemporaryThread::setCancellable() {
	m_Cancellable = true;
}