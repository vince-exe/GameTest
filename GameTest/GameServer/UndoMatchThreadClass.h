#pragma once

#include <iostream>
#include <thread>
#include <string>

class UndoMatchThreadClass {
private:
	std::shared_ptr<std::thread> t;
	bool cancellable;

public:
	static int uselessThreadCounter;

public:
	UndoMatchThreadClass(std::shared_ptr<std::thread> t, bool cancellable);

	std::shared_ptr<std::thread> getThread();

	bool isCancellable();

	void setCancellable();
};

