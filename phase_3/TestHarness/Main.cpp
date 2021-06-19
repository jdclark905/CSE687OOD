#include "TestEngine.h"
#include <iostream>

/********************/
/* Main entry point */
/********************/
int main(int argc, char *argv[])
{
	srand(time(0));

	TestEngine& testEngine = TestEngine::getInstance();
	testEngine.start();
	getchar();
	testEngine.shutdown();

	system("pause");
}
