#include "TestEngine.h"

/********************/
/* Main entry point */
/********************/
int main(int argc, char *argv[])
{
	TestEngine& testEngine = TestEngine::getInstance();
	testEngine.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	testEngine.shutdown();


	system("pause");
}
