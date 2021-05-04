
#include "UnitTest.h"
#include "TestEngine.h"
#include <iostream>
using std::cout;


// Test Engine
int main()
{
	// creating function pointers
	int(*funct_pointer1)() = &testFunction1;
	int(*funct_pointer2)() = &testFunction2;
	int(*funct_pointer3)() = &testFunction3;

	// initializing the assertion struct
	assertion assert;

	// unit test class
	UnitTest UT = UnitTest();

	// setting the log level and name
	assert.logging_level = "PassFailMessage";

	UT.AssertEquals(funct_pointer1, 0, assert, "Test1");
	UT.AssertEquals(funct_pointer2, 0, assert, "Test2");


	assert.logging_level = "PassFailMessageWithTimestamp";
	UT.AssertEquals(funct_pointer3, 0, assert, "Test3");

	TestEngine::GetInstance().Execute(UT);

	/*
	UT.GetAssertions() // returns a vector of assertions to be utilized
					   // by logEntry and passed into the Logger
	*/
}