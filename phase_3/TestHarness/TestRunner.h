#pragma once

#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include "Logger.h"
#include "Message.h"
#include "BlockingQueue.h"

#define NUM_RUNNERS 3

/* Thread-safe test runner class */
class TestRunner
{
private:
	static int _nextTestHandlerId;
	static std::mutex _mtx;

	int _myId;

	void reportStatus(HandlerStatus);

public:
	TestRunner(BlockingQueue<Message>&);
	void run();
};
