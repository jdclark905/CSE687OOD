// MattDLL.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "MattLib.h"
#include <math.h>

// Test 1
// Should pass
class Test1 : public Test
{
public:
	Test1() : Test("fibonacci(32)"){}
	virtual bool test() override
	{
		// Input parameters
		int n = 32;
		// Expected result
		int expected = (int)calculateFibonacci(n);
		// Test fibonacci function
		int result = fibonacci(n);
		if (result == expected)
		{
			_result = TEST_PASS;
		}
		else
		{
			_failMessage = "fibonacci(" + std::to_string(n) + ")";
			_failMessage += " expected " + std::to_string(expected);
			_failMessage += ", returned " + std::to_string(result);
		}
		return _result;
	}
};

// Test 2
// Should fail, calculateFibonacci is only valid up to 32nd value
// when using 1.618034 for "golden ratio" value
class Test2 : public Test
{
public:
	Test2() : Test("fibonacci(36)"){}
	virtual bool test() override
	{
		// Test fibonacci with invalid result (waste some time)
		int n = 38;
		// Invalid value to cause test failure
		int expected = (int)calculateFibonacci(n);
		int result = fibonacci(n);
		if (result == expected)
		{
			_result = TEST_PASS;
		}
		else
		{
			_failMessage = "fibonacci(" + std::to_string(n) + ")";
			_failMessage += " expected " + std::to_string(expected);
			_failMessage += ", returned " + std::to_string(result);
		}
		return _result;
	}
};

// Test 3
// Should throw handled exception
class Test3 : public Test
{
public:
	Test3() : Test("fibonacci(-1)"){}
	
	virtual bool test() override
	{
		// Test fibonacci with invalid result (waste some time)
		int n = -1;
		// Invalid value to cause test failure
		int expected = 0;
		int result = fibonacci(n);
		if (result == expected)
		{
			_result = TEST_PASS;
		}
		else
		{
			_failMessage = "fibonacci(" + std::to_string(n) + ")";
			_failMessage += " expected " + std::to_string(expected);
			_failMessage += ", returned " + std::to_string(result);
		}
		return _result;
	}
};

MATTLIBRARY_API Test* getTests()
{
	Test* test1 = new Test1();
	Test* test2 = new Test2();
	Test* test3 = new Test3();
	test1->next(test2);
	test2->next(test3);
	return test1;
}

MATTLIBRARY_API int fibonacci(int n)
{
	if (n < 0) { throw new std::invalid_argument("Fibonacci n < 0"); }
	if (n <= 1) { return n; }
	return fibonacci(n - 1) + fibonacci(n - 2);
}

MATTLIBRARY_API double calculateFibonacci(int n)
{
	double goldenRatio = 1.618034;
	return (pow(goldenRatio, n) - pow((1 - goldenRatio), n)) / sqrt(5);
}

