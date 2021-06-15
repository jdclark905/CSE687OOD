#include "TestHandler.h"
#include "windows.h"

//////////////////////////////////////
/*			TestHandler				*/
//////////////////////////////////////

void TestHandler::runner(int id)
{
	std::string runnerIdStr = "Test runner " + std::to_string(id);
	Logger::ToConsole(runnerIdStr + " started");

	while (true)
	{
		// Get next message from blocking queue
		Message msg = _testQueue.dequeue();
		Logger::ToConsole(runnerIdStr + " dequeued message:\n\t" + msg.toString());

		// If shutdown message, exit loop and stop thread
		if (msg.type() == MSG_TYPE_SHUTDOWN)
		{
			// push shutdown message back onto queue for other threads
			_testQueue.enqueue(msg);
			break;
		}

		// Check if message is test request
		if (msg.type() == MSG_TYPE_TEST_REQ)
		{
			std::string dllName = msg.getValue(MSG_ATTR_NAME_BODY);
			// will return a test result object
			try
			{
				// dll_handle = load_library "xyz.dll"
				// call "std::vector<std::string> getTestFunctions()" from dll
				// for (std::string testfn : testFunctions) {}
				// log result
			}
			catch (std::exception ex)
			{

			}
			// free_library
		}
		// otherwise unknown to test handler
		else
		{

		}
	}
	Logger::ToConsole(runnerIdStr + " stopped");
}

TestHandler::TestHandler(int poolSize) : _poolSize(poolSize)
{
	Logger::ToConsole("Test handler starting with " + std::to_string(poolSize) + " runners");
}

TestHandler::~TestHandler()
{
	shutdown();
	Logger::ToConsole("Test handler stopped");
}

void TestHandler::start()
{
	for (int i = 0; i < _poolSize; i++)
	{
		_runnerThreads.push_back(new std::thread(&TestHandler::runner, this, i));
	}
}

void TestHandler::shutdown()
{
	_running = false;
	Logger::ToConsole("Test handler shutting down");

	// Shutdown runner threads by placing shutdown message in queue
	Message msgShutdown;
	msgShutdown.type("shutdown");
	_testQueue.enqueue(msgShutdown);

	// Wait for runner threads to exit (join) and cleanup
	for (std::thread* th : _runnerThreads)
	{
		th->join();
		delete th;
	}
	_runnerThreads.clear();
	_testQueue.clear();
	Logger::ToConsole("Test handler stopped");
}

void TestHandler::enqueue(Message msg)
{
	_testQueue.enqueue(msg);
}
