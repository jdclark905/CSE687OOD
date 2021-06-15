// MattDLL.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "MattDLL.h"
#include <time.h>
/*
MATTLIBRARY_API std::vector<std::string> getFunctionNames(void)
{
	std::vector<std::string> names;
	names.push_back("testPass");
	names.push_back("testFail");
	names.push_back("testFailException");
	return names;
}
*/
// Waste a random amount of time and return true (pass)
MATTLIBRARY_API bool testPass(void)
{
	srand(time(0));
	int loopCount = rand() * 1000;
	for (int i = 0; i < loopCount; i++)
	{
		i += 1;
		i -= 1;
	}
	return true;
}

// Waste a random amount of time and return false (fail)
MATTLIBRARY_API bool testFail(void)
{
	srand(time(0));
	int loopCount = rand() * 1000;
	for (int i = 0; i < loopCount; i++)
	{
		i += 1;
		i -= 1;
	}
	return false;
}

// Waste a random amount of time before throwing an exception
MATTLIBRARY_API bool testFailException(void)
{
	srand(time(0));
	int loopCount = rand() * 1000;
	for (int i = 0; i < loopCount; i++)
	{
		i += 1;
		i -= 1;
	}
	loopCount = divide(loopCount, 0);
	return false;
}

// Division, int
MATTLIBRARY_API int divide(int a, int b)
{
	return a / b;
}