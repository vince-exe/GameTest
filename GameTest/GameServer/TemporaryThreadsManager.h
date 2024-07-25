#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include "TemporaryThread.h"

class TemporaryThreadsManager {
private:
	std::vector<TemporaryThread> m_Vec;
	mutable std::mutex m_Mutex;
	size_t m_uselessCounter;

public:
	TemporaryThreadsManager();

	void push(TemporaryThread&& val);
	
	size_t size() const;

	void pop();

	const TemporaryThread& back() const;

	void increaseUselessCounter();

	size_t clearUselessThreads();
};

