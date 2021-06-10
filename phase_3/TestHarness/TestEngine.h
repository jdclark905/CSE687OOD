#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.h"
#include "Message.h"
#include "BlockingQueue.h"

/* Thread-safe singleton class to run test harness */
class TestEngine
{
private:
	struct 

	TestEngine();
	~TestEngine();

	static TestEngine* _instance;
	static std::mutex _mtx;

	BlockingQueue<Message> _msgQueue;		// Queue for messages received by test runners
	BlockingQueue<std::string > _testQueue;	// Queue for test requests that runners wait on
	std::thread _msgHandler;				// Message handler thread
	std::thread _clientHandler;				// Client connection handler thread
	std::vector<std::thread> _testHandlers;	// Test handler thread pool

	void runMsgHandler();					// Message handler thread function
	void runClientHandler();				// Client connection handler funciton
	void runTestHandler(const int);			// Test handler thread function
	void postMsg(Message);

public:
	// Prevent copying and assignment
	TestEngine(const TestEngine&) = delete;
	TestEngine& operator=(const TestEngine&) = delete;

	// Get singleton instance (thread-safe)
	static TestEngine* getInstance();

	// Start test engine
	void start();
};
