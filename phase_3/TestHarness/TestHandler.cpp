#include "TestHandler.h"

/* TestRunner */
TestRunner::TestRunner(TestHandler& testHandler, int id) : _testHandler(testHandler), _id(id), _running(false)
{
	_thread = new std::thread(&TestRunner::runner, this);
}

void TestRunner::runner()
{
	_running = true;
	while (!_testHandler._shutdown)
	{
		// Code to get test command from message and run test
		// Get name of DLL, load and call test function, which
		// will return a test result object
		try
		{
			// load "xyz.dll"
			// call "std::vector<std::string> getTestFunctions()" from dll
			// for (std::string testfn : testFunctions) {}
		}
		catch (std::exception ex)
		{

		}
		// log test result
	}
}

void TestRunner::join()
{
	_thread->join();
}

/* TestHandler */
TestHandler::TestHandler(int poolSize) : _poolSize(poolSize), _shutdown(false) {}

TestHandler::~TestHandler()
{
	shutdown();
}

void TestHandler::start()
{
	for (int i = 0; i < _poolSize; i++)
	{
		_runners.push_back(new TestRunner(*this, i));
	}
}

void TestHandler::shutdown()
{
	_shutdown = true;
	for (TestRunner* runner : _runners)
	{
		runner->join();
		delete runner;
	}
	_runners.clear();
	_testQueue.clear();
}

void TestHandler::enqueue(Message msg)
{
	_testQueue.enqueue(msg);
}
