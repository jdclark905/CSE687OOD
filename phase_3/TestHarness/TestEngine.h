#pragma once

#include <string>
#include "Logger.h"
#include "TestHandler.h"

/* Thread-safe singleton class to run test harness */
class TestEngine
{
private:
	TestEngine();
	~TestEngine();

	static TestEngine* _instance;
	static std::mutex _mtx;

	TestHandler _testHandler;

public:
	// Prevent copying and assignment
	TestEngine(const TestEngine&) = delete;
	TestEngine& operator=(const TestEngine&) = delete;

	// Get singleton instance (thread-safe)
	static TestEngine* getInstance();

	// Start test engine
	void start();
};
