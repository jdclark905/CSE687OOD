// MattDLL.cpp : Defines the exported functions for the DLL application.
//

#include "pch.h"
#include "MattLib.h"
/*
MATTLIBRARY_API std::vector<std::string> getTestNames()
{
	
	//if (id == 0){ return "testPass"; }
	//if (id == 1){ return "testFail"; }
	//if (id == 2){ return "testFailException"; }
	//return nullptr;
	
	std::vector<std::string> names;
	names.push_back("testPass");
	names.push_back("testFail");
	names.push_back("testFailException");
	return names;
}
*/

class Test1 : public ITest
{
public:
	Test1(const std::string& name) : ITest(name){}
	virtual bool test() override
	{
		int n = 9;
		int expected = 34;
		_message = "fibonacci(" + std::to_string(n) + ")";
		_message += ", expected " + std::to_string(expected);
		int result = fibonacci(n);
		if (result == expected)
		{
			_pass = true;
		}
		_message += ", returned " + std::to_string(result);
		return _pass;
	}
};

class Test2 : public ITest
{
public:
	Test2(const std::string& name) : ITest(name){}
	virtual bool test() override
	{
		int a = 3, b = 2;
		float expected = 1.5;
		_message = "divide(" + std::to_string(a) + "," + std::to_string(b) + ")";
		_message += ", expected " + std::to_string(expected);
		float result = divide(a, b);
		if (result == expected)
		{
			_pass = true;
		}
		_message += ", returned " + std::to_string(result);
		return _pass;
	}
};

class Test3 : public ITest
{
public:
	Test3(const std::string& name) : ITest(name){}
	virtual bool test() override
	{
		int a = 3, b = 0;
		float expected = 3.0;
		_message = "divide(" + std::to_string(a) + "," + std::to_string(b) + ")";
		_message += ", expected " + std::to_string(expected);
		float result = divide(a, b);
		if (result == expected)
		{
			_pass = true;
		}
		_message += ", returned " + std::to_string(result);
		return _pass;
	}
};

MATTLIBRARY_API ITest* getTests()
{
	ITest* test1 = new Test1("Test1");
	ITest* test2 = new Test2("Test2");
	ITest* test3 = new Test3("Test3");
	test1->next(test2);
	test2->next(test3);
	return test1;
}

MATTLIBRARY_API int fibonacci(int n)
{
	if (n <= 1) { return n; }
	return fibonacci(n - 1) + fibonacci(n - 2);
}

// Multiplication, return a * b
MATTLIBRARY_API int multiply(int a, int b)
{
	return a * b;
}

// Division, return a / b
MATTLIBRARY_API int divide(int a, int b)
{
	return a / b;
}