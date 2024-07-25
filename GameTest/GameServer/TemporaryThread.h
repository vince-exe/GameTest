#pragma once

#include <iostream>
#include <thread>
#include <string>

class TemporaryThread {
private:
	std::shared_ptr<std::thread> m_Thread;
	bool m_Cancellable;

public:
	TemporaryThread(std::shared_ptr<std::thread> t, bool cancellable);

	std::shared_ptr<std::thread> getThread() const;

	bool isCancellable() const;

	void setCancellable();
};

