
#include <vector>
#include <string>
#include <exception>

#ifndef UNIT_TEST
#define UNIT_TEST

using std::vector;
using std::string;

enum LogLevel {
	PassFailOnly,
	PassFailMessage,
	PassFailMessageWithTimestamp
};

struct TestResult
{
	bool result;
	std::string message = "";
	std::string error = "";
};

// callable object that does not take any parameters
struct assertion {
	string assertion_type = "test";
	string function_name = "func";
	TestResult testResult;
	LogLevel logLevel = PassFailOnly;
	int (*fnptr)();
};

class UnitTest
{

private:
	vector<assertion> asserts_;

public:

	// Test method to override
	virtual bool execute() = 0;

	// assertion - exeuction function
	static void AssertEquals(int (*function)(), int test, assertion assert);

	// getter for retrieving tests and their results
	// vector<assertion> GetAssertions();

};

#endif

