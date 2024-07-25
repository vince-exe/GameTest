#pragma once

#include <iostream>
#include <queue>
#include <shared_mutex>

template<class T1>
class ThreadSafeQueue {
private:
	std::queue<T1> m_Queue;
	mutable std::shared_mutex m_Mutex;

public:
	ThreadSafeQueue() = default;

	void push(T1 val);

	void pop();

	typename std::queue<T1>::const_reference front() const;

	bool empty() const;
};

template<class T1>
void ThreadSafeQueue<T1>::push(T1 val) {
	std::unique_lock<std::shared_mutex> lock(m_Mutex);
	m_Queue.push(val);
}

template<class T1>
void ThreadSafeQueue<T1>::pop() {
	std::unique_lock<std::shared_mutex> lock(m_Mutex);
	m_Queue.pop();
}

template<class T1>
typename std::queue<T1>::const_reference ThreadSafeQueue<T1>::front() const {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_Queue.front();
}

template<class T1>
bool ThreadSafeQueue<T1>::empty() const {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_Queue.empty();
}