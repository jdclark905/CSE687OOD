#pragma once

#include <atomic>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.h"
#include "Message.h"
#include "BlockingQueue.h"

#define DEFAULT_POOL_SIZE 3

class TestMessage
{

};

//	Test handler with thread pool
class TestHandler
{
private:
	std::atomic_bool _stop;
	BlockingQueue<Message> _testQueue;
	std::vector<std::thread> _runners;
	void runner_thread();

public:
	// Prevent copying and assignment
	TestHandler(const TestHandler&) = delete;
	TestHandler& operator=(const TestHandler&) = delete;

	TestHandler();

	void addTest()
};
