#include "TestEngine.h"
#include "Logger.h"

TestEngine TestEngine::_instance;

TestEngine& TestEngine::getInstance()
{
	return _instance;
}

TestEngine::TestEngine() : _running(false), _testHandler(_requestQueue, _responseQueue), _clientHandler(_requestQueue, _responseQueue)
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
	// test DLL load functionality
	/*
	Message msg;
	msg.from(MsgAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT));
	msg.to(MsgAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT));
	msg.author("Matt");
	msg.type(MSG_TYPE_TEST_REQ);
	msg.body("MattLib - Copy.dll");
	_testHandler.enqueue(msg);
	msg.body("MattLib - Copy (2).dll");
	_testHandler.enqueue(msg);
	msg.body("MattLib - Copy (3).dll");
	_testHandler.enqueue(msg);
	*/

	_testHandler.start();
	_clientHandler.start();
	_running = true;
}

void TestEngine::shutdown()
{
	_testHandler.shutdown();
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
