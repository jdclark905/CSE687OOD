#include "TestEngine.h"
#include "Logger.h"

TestEngine* TestEngine::_instance = nullptr;
std::mutex TestEngine::_mtx;

TestEngine::TestEngine() : _msgQueue(), _testQueue(), _runnerPool()
{
}

void TestEngine::testRunner(int id)
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

void TestEngine::start()
{

}