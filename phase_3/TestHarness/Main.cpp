
#include "UnitTest.h"
#include "TestEngine.h"
#include <iostream>
using std::cout;


// Test Engine
int main()
{
	TestEngine::GetInstance().Execute();

	/*
	UT.GetAssertions() // returns a vector of assertions to be utilized
					   // by logEntry and passed into the Logger
	*/
}