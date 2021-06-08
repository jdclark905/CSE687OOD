#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.h"
#include "Message.h"
#include "BlockingQueue.h"

#define NUM_RUNNERS 3

/* Thread-safe singleton class to run test harness */
class TestEngine
{
private:
	TestEngine();
	~TestEngine();

	static TestEngine* _instance;
	static std::mutex _mtx;
	BlockingQueue<Message> _msgQueue;		// Queue for messages received by test runners
	BlockingQueue<Message> _testQueue;		// Queue for test requests that runners wait on
	std::vector<std::thread> _runnerPool;	// Test runner thread pool

	void testRunner(int id);				// Test runner thread function

public:
	// Prevent copying and assignment
	TestEngine(const TestEngine&) = delete;
	TestEngine& operator=(const TestEngine&) = delete;

	// Get singleton instance (thread-safe)
	static TestEngine* getInstance();

	// Start test engine
	void start();
};
