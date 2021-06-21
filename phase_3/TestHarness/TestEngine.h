#pragma once

#include "TestHandler.h"
#include "ClientHandler.h"

/* Thread-safe singleton class to run test harness */
class TestEngine
{
private:
	TestEngine();
	~TestEngine();

	static TestEngine _instance;
	BlockingQueue<Message> _requestQueue;
	BlockingQueue<Message> _responseQueue;
	TestHandler _testHandler;
	ClientHandler _clientHandler;
	bool _running;

public:
	// Prevent copying and assignment
	TestEngine(const TestEngine&) = delete;
	TestEngine& operator=(const TestEngine&) = delete;

	// Get singleton instance (thread-safe)
	static TestEngine& getInstance();

	// Start test engine
	void start();
	// Shutdown test engine
	void shutdown();

};
