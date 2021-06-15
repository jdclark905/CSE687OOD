#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include "Logger.h"
#include "BlockingQueue.h"
#include "Message.h"

#define DEFAULT_POOL_SIZE 3

class TestHandler
{
private:
	int _poolSize;
	bool _running;
	BlockingQueue<Message> _testQueue;
	//std::vector<TestRunner*> _runners;
	//friend class TestRunner;
	std::vector<std::thread*> _runnerThreads;
	void runner(int id);

public:
	TestHandler(int poolSize = DEFAULT_POOL_SIZE);
	~TestHandler();
	void start();
	void shutdown();
	void enqueue(Message msg);
};

