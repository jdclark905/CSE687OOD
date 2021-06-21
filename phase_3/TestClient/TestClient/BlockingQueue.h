#pragma once

#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>

template <typename T>
class BlockingQueue
{
public:
	// Default constructor
	BlockingQueue() {}
	// Move constructor
	BlockingQueue(BlockingQueue<T>&& bq);
	// Copy constructor
	BlockingQueue(const BlockingQueue<T>&) = delete;
	// Assignment operator
	BlockingQueue<T>& operator=(const BlockingQueue<T>&) = delete;

	// Get first item from queue
	T dequeue();

	// Add item to queue
	void enqueue(const T& item);

	// Point to the front of the queue
	T& front();

	// Clear the queue
	void clear();

	// Get the size of the queue
	size_t size();

private:
	std::queue<T> _queue;
	std::mutex _mutex;
	std::condition_variable _condv;
};

// Move constructor
template <typename T>
BlockingQueue<T>::BlockingQueue(BlockingQueue<T>&& bq)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_queue = bq._queue;
	while (bq._queue.size() > 0)
	{
		bq._queue.pop();
	}
}

// Get first item from queue
template <typename T>
T BlockingQueue<T>::dequeue()
{
	std::unique_lock<std::mutex> lock(_mutex);

	if (_queue.size() > 0)
	{
		T item = _queue.front();
		_queue.pop();
		return item;
	}

	while (_queue.size() == 0)
	{
		_condv.wait(lock, [this]() {return _queue.size() > 0; });
	}
	T item = _queue.front();
	_queue.pop();
	return item;
}

// Add item to queue
template <typename T>
void BlockingQueue<T>::enqueue(const T& item)
{
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_queue.push(item);
	}
	_condv.notify_one();
}

// Point to the front of the queue
template <typename T>
T& BlockingQueue<T>::front()
{
	std::lock_guard<std::mutex> lock(_mutex);
	if (_queue.size() > 0)
	{
		return _queue.front();
	}
	throw std::exception("access to an empty queue");
}

// Clear the queue
template <typename T>
void BlockingQueue<T>::clear()
{
	std::lock_guard<std::mutex> lock(_mutex);
	while (_queue.size() > 0)
	{
		_queue.pop();
	}
}

// Get the size of the queue
template <typename T>
size_t BlockingQueue<T>::size()
{
	std::lock_guard<std::mutex> lock(_mutex);
	return _queue.size();
}
