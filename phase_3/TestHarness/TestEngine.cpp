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
	_testHandler.start();
	_running = true;
}

void TestEngine::shutdown()
{
	_testHandler.shutdown();
	_running = false;
}
