
#include <vector>
#include <string>

#ifndef UNIT_TEST
#define UNIT_TEST

using std::vector;
using std::string;

// callable object that does not take any parameters
int testFunction1();
int testFunction2();
int testFunction3();

/*

*/

struct assertion {
	string assertion_type = "test";
	string function_name = "func";
	bool result = false;
	string logging_level = "PassFailOnly";
};


class UnitTest
{
public:

	// default constructor
	UnitTest();

	// assertion - exeuction function
	void AssertEquals(
		int (*function)(), 
		int test, 
		assertion assert,
		string func_name
	);

	// void SetAssertions(vector<assertions> functions);

	// getter for retrieving tests and their results
	vector<assertion> GetAssertions();

private:
	vector<assertion> asserts_;
};

#endif

