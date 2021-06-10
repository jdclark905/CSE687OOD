#include "TestRunner.h"
#include "Message.h"

int TestRunner::_nextTestHandlerId = 0;

TestRunner::TestRunner(BlockingQueue<Message>& q)
{
	std::lock_guard<std::mutex> lock(_mtx);
	_myId = _nextTestHandlerId++;
}

