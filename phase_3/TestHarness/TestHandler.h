#pragma once

#include <atomic>
#include <string>
#include <thread>
#include <vector>
#include "Logger.h"
#include "BlockingQueue.h"
#include "Message.h"

#define DEFAULT_POOL_SIZE 3

class TestRunner
{
private:
	TestHandler& _testHandler;
	std::thread* _thread;
	std::atomic<bool> _running;
	const int _id;

	void runner();

public:
	TestRunner(TestHandler& testHandler, int id);
	void join();
};

class TestHandler
{
private:
	int _poolSize;
	BlockingQueue<Message> _testQueue;
	std::vector<TestRunner*> _runners;
	std::atomic<bool> _shutdown;
	friend class TestRunner;

public:
	TestHandler(int poolSize = DEFAULT_POOL_SIZE);
	void start();
	void shutdown();
	void enqueue(Message msg);
	bool running();
};

