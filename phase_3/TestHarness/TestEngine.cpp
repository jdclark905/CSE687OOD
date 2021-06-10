#include "TestEngine.h"
#include "Logger.h"

TestEngine* TestEngine::_instance = nullptr;
std::mutex TestEngine::_mtx;

TestEngine::TestEngine() : _msgQueue(), _testQueue(), _testHandlers()
{
}

TestEngine* TestEngine::getInstance()
{
	std::lock_guard<std::mutex> lock(_mtx);
	if (_instance == nullptr)
	{
		_instance = new TestEngine();
	}
	return _instance;
}

void TestEngine::runMsgHandler()
{

}

void TestEngine::runClientHandler()
{
	// Initialize listening socket

	// Listen for connection requests


}

void TestEngine::runTestHandler(const int id)
{
	
}

void TestEngine::postMsg(const std::string &source, MessageType msgType, const std::string &body)
{

}

void TestEngine::start()
{
	_msgHandler = std::thread(&runMsgHandler);
	_clientHandler = std::thread(&runClientHandler);
	for (int i = 0; i < 3; i++)
	{
		_testHandlers.push_back(std::thread(&runTestHandler, i));
	}
}