#include "TestRunner.h"
#include "Message.h"

TestHandler::TestHandler()
{
	_stop = false;
}

void TestHandler::runner_thread()
{
	Message msg;
	while (!_stop)
	{
		msg = _testQueue.dequeue();
	}
}