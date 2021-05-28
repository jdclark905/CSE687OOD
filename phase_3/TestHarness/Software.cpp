#include "Software.h"
#include <string>

int testFunction1()
{
	return 0;
}

int testFunction2()
{
	// test failed
	return -1;
}

int testFunction3()
{
	// exception raised
	std::string("abc").substr(10);
	return 0;
}
