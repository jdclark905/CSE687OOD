#include "TestEngine.h"
#include "Logger.h"

TestEngine TestEngine::_instance;

TestEngine& TestEngine::getInstance()
{
	return _instance;
}

TestEngine::TestEngine() : _running(false),
	_testHandler(_requestQueue, _responseQueue),
	_clientHandler(_requestQueue, _responseQueue)
{
	
}

TestEngine::~TestEngine()
{
	if (_running)
	{
		shutdown();
	}
}

void TestEngine::start()
{
	_testHandler.start();
	_clientHandler.start();
	_running = true;
}

void TestEngine::shutdown()
{
	_testHandler.shutdown();
	_clientHandler.shutdown();
	_running = false;
}

/////////////////////////////////////////////////
//	Main entry point
/////////////////////////////////////////////////
int main(int argc, char *argv[])
{
	srand(time(0));
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	TestEngine& testEngine = TestEngine::getInstance();
	testEngine.start();
	getchar();
	testEngine.shutdown();

	system("pause");
}
