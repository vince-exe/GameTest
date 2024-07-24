#pragma once

#include <iostream>
#include <unordered_map>
#include <shared_mutex>

template<class T1, class T2>
class ThreadSafeUnorderedMap {
private:
	std::unordered_map<T1, T2> m_unMap;
	mutable std::shared_mutex m_Mutex;

public:
	ThreadSafeUnorderedMap() = default;

	void insert(T1 key, T2 value);
	 
	T2 get(const T1& value) const;

	size_t size() const;

	size_t erase(const T1& key);

	typename std::unordered_map<T1, T2>::const_iterator find(const T1& key) const;

	typename std::unordered_map<T1, T2>::const_iterator end() const;
};

template<class T1, class T2>
void ThreadSafeUnorderedMap<T1, T2>::insert(T1 key, T2 value) {
	std::unique_lock<std::shared_mutex> lock(m_Mutex);
	m_unMap[key] = value;
}

template<class T1, class T2>
T2 ThreadSafeUnorderedMap<T1, T2>::get(const T1& key) const {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_unMap.at(key);
}

template<class T1, class T2>
size_t ThreadSafeUnorderedMap<T1, T2>::size() const {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_unMap.size();
}

template<class T1, class T2>
size_t ThreadSafeUnorderedMap<T1, T2>::erase(const T1& key) {
	std::unique_lock<std::shared_mutex> lock(m_Mutex);
	return m_unMap.erase(key);
}

template<class T1, class T2>
typename std::unordered_map<T1, T2>::const_iterator ThreadSafeUnorderedMap<T1, T2>::find(const T1& key) const {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_unMap.find(key);
}

template<class T1, class T2>
typename std::unordered_map<T1, T2>::const_iterator ThreadSafeUnorderedMap<T1, T2>::end() const {
	std::shared_lock<std::shared_mutex> lock(m_Mutex);
	return m_unMap.cend();
}