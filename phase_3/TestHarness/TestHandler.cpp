#include "TestHandler.h"
#include "Test.h"

//////////////////////////////////////
/*			TestHandler				*/
//////////////////////////////////////

#define RUNNER_MSG(id, msg) "Test runner " + TO_STR(id) + ": " + msg

TestHandler::TestHandler(BlockingQueue<Message>& requestQueue, BlockingQueue<Message>& responseQueue, int poolSize)
: _requestQueue(requestQueue), _responseQueue(responseQueue), _poolSize(poolSize)
{
	Logger::ToConsole("Test handler starting with " + TO_STR(poolSize) + " runners");
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
	msgShutdown.type(MSG_TYPE_SHUTDOWN);
	_requestQueue.enqueue(msgShutdown);
	
	// Wait for runner threads to exit (join) and cleanup
	for (std::thread* th : _runnerThreads)
	{
		th->join();
		delete th;
	}
	_runnerThreads.clear();
	_requestQueue.clear();
	Logger::ToConsole("Test handler stopped");
}

void TestHandler::enqueue(Message msg)
{
	_requestQueue.enqueue(msg);
}

void TestHandler::runner(int id)
{
	typedef Test* (__cdecl *GetTestObj)(void);
	typedef bool (__cdecl *testProc)(void);
	HINSTANCE hDLL = nullptr;
	GetTestObj getTestObj;
	bool testResult;
	bool causedException;
	LogLevel logLevel;
	std::string log = "Runner " + TO_STR(id) + ": ";

	Logger::ToConsole(RUNNER_MSG(id, "started"));

	while (true)
	{
		//Logger::ToConsole(runnerIdStr + ": sleeping for " + std::to_string(sleepTime[id]) + " ms");
		std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime[id]));
		Logger::ToConsole(RUNNER_MSG(id, "ready"));

		// Get next message from blocking queue
		Message msg = _requestQueue.dequeue();
		Logger::ToConsole(RUNNER_MSG(id, "dequeued message:\n" + msg.printString()));

		// If shutdown message, exit loop and stop thread
		if (msg.type() == MSG_TYPE_SHUTDOWN)
		{
			// push shutdown message back onto queue for other threads
			_requestQueue.enqueue(msg);
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
				Logger::ToConsole(RUNNER_MSG(id, "loaded DLL " + dllName));
				
				// Get pointer to "getTestName" function in DLL
				getTestObj = (GetTestObj)GetProcAddress(hDLL, FN_GET_TESTS);
				if (getTestObj != NULL)
				{
					// Set logging level
					if (msg.getValue(MSG_ATTR_NAME_LOGLEVEL) == MSG_LOGLVL_PFD)
					{
						logLevel = PassFailDetail;
					}
					else
					{
						logLevel = PassFailOnly;
					}

					// Retrieve test function names and run
					Test* testObjPrev = nullptr;
					int numTests = 0;
					for (Test* testObj = getTestObj(); testObj != nullptr; testObj = testObj->next())
					{
						// Free memory of previous test instance
						if (testObjPrev != nullptr)
						{
							delete testObjPrev;
						}

						// Run test
						numTests++;
						testResult = false;
						causedException = false;
						try
						{
							testResult = testObj->test();
						}
						catch (...)
						{
							causedException = true;
						}

						// Log results
						std::string consoleMsg = RUNNER_MSG(id, dllName + " test(" + TO_STR(numTests) + "): " + testObj->name() + " returned " + (testResult ? "pass" : "fail"));
						msg.setAttribute(MSG_ATTR_NAME_RESULT, testResult ? "pass" : "fail");
						if (!testResult && logLevel == PassFailDetail)
						{
							if (causedException)
							{
								msg.setAttribute(MSG_ATTR_NAME_DETAIL, "exception occurred");
							}
							else
							{

								msg.setAttribute(MSG_ATTR_NAME_DETAIL, testObj->message());
							}
						}
						consoleMsg += " - " + msg.getValue(MSG_ATTR_NAME_DETAIL);
						MsgAddress addr = msg.from();
						msg.from(msg.to());
						msg.to(addr);
						msg.author("Test Harness runner " + TO_STR(id));
						msg.type(MSG_TYPE_TEST_RESP);
						Logger::ToConsole(consoleMsg);

						// Send result to client
						_responseQueue.enqueue(msg);

						// Set previous test object to this one, for loop iteration delete
						testObjPrev = testObj;
						// Last test, delete and exit loop
						if (testObj->next() == nullptr)
						{
							delete testObj;
							break;
						}
					};

					// Done running tests, send response to client
				}
				else
				{
					Logger::ToConsole(RUNNER_MSG(id, "DLL does not support " + FN_GET_TESTS));
				}

				// free_library
				Logger::ToConsole(RUNNER_MSG(id, "freeing library " + dllName));
				FreeLibrary(hDLL);
			}
			else
			{
				Logger::ToConsole(RUNNER_MSG(id, "error loading DLL " + dllName));
			}
		}
		// otherwise unknown to test handler
		else
		{
			Logger::ToConsole("Unhandled message type received");
		}
		sleepTime[id] = rand() % 20 + 10;
	}
	Logger::ToConsole(RUNNER_MSG(id, "stopped"));
}
