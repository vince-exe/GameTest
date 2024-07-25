#include "TemporaryThreadsManager.h"

TemporaryThreadsManager::TemporaryThreadsManager() {
	m_uselessCounter = 0;
}

void TemporaryThreadsManager::push(TemporaryThread&& val) {
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Vec.push_back(val);
}

size_t TemporaryThreadsManager::size() const {
	return m_Vec.size();
}

void TemporaryThreadsManager::pop() {
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_Vec.pop_back();
}

const TemporaryThread& TemporaryThreadsManager::back() const {
	std::lock_guard<std::mutex> lock(m_Mutex);
	return m_Vec.back();
}

void TemporaryThreadsManager::increaseUselessCounter() {
	std::lock_guard<std::mutex> lock(m_Mutex);
	m_uselessCounter++;

	auto it = m_Vec.begin();
	int i = 0;
	// Here it's not used the back() function to assign setCancellable() because, mean while a new good thread may be added to the vec and the back() function will cause an unexpected behavior.
	while (it != m_Vec.end() && i < m_uselessCounter) {
		it->setCancellable();
		i++;
		it++;
	}
}

size_t TemporaryThreadsManager::clearUselessThreads() {
	std::lock_guard<std::mutex> lock(m_Mutex);
	size_t oldSize = m_Vec.size();

	m_Vec.erase(std::remove_if(m_Vec.begin(), m_Vec.end(), [](TemporaryThread& tThread) {
		return tThread.isCancellable();
	}),
	m_Vec.end());
	m_uselessCounter = 0;

	return oldSize - m_Vec.size();
}
