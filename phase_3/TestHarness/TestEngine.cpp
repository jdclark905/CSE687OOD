#include "TestEngine.h"
#include "Logger.h"

TestEngine TestEngine::_instance;

TestEngine& TestEngine::getInstance()
{
	return _instance;
}

TestEngine::TestEngine() : _running(false)
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
	Message msg;
	msg.from(MsgAddress("localhost", 10000));
	msg.to(MsgAddress("localhost", 10000));
	msg.author("Matt");
	msg.type(MSG_TYPE_TEST_REQ);
	msg.body("MattLib - Copy.dll");
	_testHandler.enqueue(msg);
	msg.body("MattLib - Copy (2).dll");
	_testHandler.enqueue(msg);
	msg.body("MattLib - Copy (3).dll");
	_testHandler.enqueue(msg);

	_testHandler.start();
	_running = true;
}

void TestEngine::shutdown()
{
	_testHandler.shutdown();
	_running = false;
}
