
#include "UnitTest.h"

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

	cout
		<< "Test Details (boolean results): \n\n"

		<< "Logging level: " << assert.logging_level << "\n\n"

		<< UT.GetAssertions()[0].function_name << ": "
		<< UT.GetAssertions()[0].result << " : "
		<< UT.GetAssertions()[0].assertion_type
		<< "\n"


		<< UT.GetAssertions()[1].function_name << ": "
		<< UT.GetAssertions()[1].result << " : "
		<< UT.GetAssertions()[1].assertion_type
		<< "\n\n";

	assert.logging_level = "PassFailMessageWithTimestamp";
	UT.AssertEquals(funct_pointer3, 0, assert, "Test3");

	cout 
		<< "Logging level: " << assert.logging_level << "\n\n"

		<< UT.GetAssertions()[2].function_name << ": "
		<< UT.GetAssertions()[2].result << " : "
		<< UT.GetAssertions()[2].assertion_type
		<< "\n\n";

	// info for logger
	for (int test=0; test < UT.GetAssertions().size(); test++)
	{
		cout
			<< UT.GetAssertions()[test].function_name << " : "
			<< UT.GetAssertions()[test].logging_level << " : "
			<< UT.GetAssertions()[test].result << "\n";
	}

	/*
	UT.GetAssertions() // returns a vector of assertions to be utilized
					   // by logEntry and passed into the Logger
	*/
}