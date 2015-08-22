#pragma once

#include <queue>
#include <mutex>

template <typename T>
class object_pool {
	std::queue<T*> pool;
	std::mutex mutex;
public:
	object_pool(size_t num_objects = 0);
	~object_pool();
	void allocate(size_t num_objects);
	T* pop();
	void push(T* object);
	size_t num_available_objects();
};


template <typename T>
object_pool<T>::object_pool(size_t num_objects) {
	allocate(num_objects);
}

template <typename T>
object_pool<T>::~object_pool() {
	while (!pool.empty()) {
		T* object = pool.front();
		pool.pop();
		delete object;
		object = nullptr;
	}
	// TODO how are objects deleted that are currently in use
}

template <typename T>
void object_pool<T>::allocate(size_t num_objects) {
	mutex.lock();
	size_t num_to_be_added = num_objects - pool.size();
	for (unsigned int i = 0; i < num_to_be_added; i++) {
		pool.push(new T);
	}
	mutex.unlock();
}

template <typename T>
T* object_pool<T>::pop() {
	mutex.lock();
	T* object = pool.front();
	pool.pop();
	mutex.unlock();
	return object;
}

template <typename T>
void object_pool<T>::push(T* object) {
	mutex.lock();
	pool.push(object);
	mutex.unlock();
}

template <typename T>
size_t object_pool<T>::num_available_objects() {
	mutex.lock();
	size_t size = pool.size();
	mutex.unlock();
	return size;
}