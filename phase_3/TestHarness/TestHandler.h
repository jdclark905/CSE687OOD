#pragma once

#include <string>
#include <thread>
#include <vector>
#include "Logger.h"
#include "BlockingQueue.h"
#include "Message.h"

#define DEFAULT_POOL_SIZE 3
#define FN_GET_TEST_NAMES "getTestNames"
#define FN_GET_TEST_OBJ "getTestObj"
#define FN_GET_TESTS "getTests"

class TestHandler
{
private:
	int _poolSize;
	bool _running;
	BlockingQueue<Message> _testQueue;
	std::vector<std::thread*> _runnerThreads;
	void runner(int id);
	int* sleepTime;

public:
	TestHandler(int poolSize = DEFAULT_POOL_SIZE);
	~TestHandler();
	void start();
	void shutdown();
	BlockingQueue<Message>* queue();
	void enqueue(Message msg);
};

