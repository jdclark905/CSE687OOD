#include "TestHandler.h"
#include "ITest.h"

#ifndef _WINDOWS_
#include <Windows.h>
#endif

//////////////////////////////////////
/*			TestHandler				*/
//////////////////////////////////////

void TestHandler::runner(int id)
{
	typedef ITest* (__cdecl *GetTestObj)(void);
	typedef bool (__cdecl *testProc)(void);
	HINSTANCE hDLL = nullptr;
	GetTestObj getTestObj;
	testProc test;

	std::string runnerIdStr = "Test runner " + std::to_string(id);
	Logger::ToConsole(runnerIdStr + ": starting");

	while (true)
	{
		//Logger::ToConsole(runnerIdStr + ": sleeping for " + std::to_string(sleepTime[id]) + " ms");
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime[id]));
		Logger::ToConsole(runnerIdStr + ": ready");

		// Get next message from blocking queue
		Message msg = _testQueue.dequeue();
		Logger::ToConsole(runnerIdStr + ": dequeued message: " + msg.toString());

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
			// Get name of DLL to test from message
			std::string dllName = msg.body();
			
			// Load DLL
			hDLL = LoadLibrary(dllName.c_str());
			if (hDLL != NULL)
			{
				Logger::ToConsole(runnerIdStr + ": loaded DLL " + dllName);
				
				// Get pointer to "getTestName" function in DLL
				getTestObj = (GetTestObj)GetProcAddress(hDLL, FN_GET_TESTS);
				if (getTestObj != NULL)
				{
					Logger::ToConsole(runnerIdStr + ": DLL function " + FN_GET_TESTS + " valid");
					// Retrieve test function names and run
					ITest* testObjPrev = nullptr;
					for (ITest* testObj = getTestObj(); testObj != nullptr; testObj = testObj->next())
					{
						if (testObjPrev != nullptr)
						{
							delete testObjPrev;
						}
						Logger::ToConsole(runnerIdStr + ": test funciton " + testObj->name());
						/*
						test = (testProc)GetProcAddress(hDLL, testName.c_str());
						if (test)
						{*/
							try
							{
								//bool result = test();
								bool result = testObj->test();
								Logger::ToConsole(runnerIdStr + ": function " + testObj->name() + " returned " + (result ? "pass" : "fail"));
							}
							catch (...)
							{
								Logger::ToConsole(runnerIdStr + ": exception while testing function " + testObj->name());
							}
						/*}
						else
						{
							Logger::ToConsole(runnerIdStr + ": error getting procaddress of " + testName);
						}*/
							testObjPrev = testObj;
					};

				}
				else
				{
					Logger::ToConsole(runnerIdStr + ": DLL does not support " + FN_GET_TESTS);
				}

				// free_library
				Logger::ToConsole(runnerIdStr + ": freeing library " + dllName);
				FreeLibrary(hDLL);
			}
			else
			{
				Logger::ToConsole(runnerIdStr + ": error loading DLL " + dllName);
			}
		}
		// otherwise unknown to test handler
		else
		{

		}
		sleepTime[id] = rand() % 20 + 10;
	}
	Logger::ToConsole(runnerIdStr + ": stopped");
}

TestHandler::TestHandler(int poolSize) : _poolSize(poolSize)
{
	Logger::ToConsole("Test handler starting with " + std::to_string(poolSize) + " runners");
	sleepTime = new int[_poolSize];
}

TestHandler::~TestHandler()
{
	shutdown();
	delete[] sleepTime;
	Logger::ToConsole("Test handler stopped");
}

void TestHandler::start()
{
	for (int i = 0; i < _poolSize; i++)
	{
		sleepTime[i] = rand() % 20 + 10;
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
