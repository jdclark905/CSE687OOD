// MattDLL.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "MattLib.h"

MATTLIBRARY_API char* getTestName(int id)
{
	if (id == 0){ return "testPass"; }
	if (id == 1){ return "testFail"; }
	if (id == 2){ return "testFailException"; }
	return nullptr;
}

// Waste a random amount of time and return true (pass)
MATTLIBRARY_API bool testPass(void)
{
	return true;
}

// Waste a random amount of time and return false (fail)
MATTLIBRARY_API bool testFail(void)
{
	return false;
}

// Waste a random amount of time before throwing an exception
MATTLIBRARY_API bool testFailException(void)
{
	return divide(1,0) == 1;
}

// Division, int
MATTLIBRARY_API int divide(int a, int b)
{
	return a / b;
}