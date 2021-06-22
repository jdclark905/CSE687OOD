#include "pch.h"
//#include "stdafx.h"
#include "TestHarness.h"



TESTHARNESS_API bool beTrue(int x)
{
	if (x < x)
	{
		return true;
	}
	else
	{
		//just cause this always needs to return true
		return true;
	}
}

TESTHARNESS_API bool beFalse(int x)
{
	if (x > x)
	{
		return false;
	}
	else
	{
		//just cause this always needs to return false
		return false;
	}
}

TESTHARNESS_API bool beEqual(int x)
{
	if (x == x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

class TrueTest : public Test
{
public:
	TrueTest() : Test("beTrueTest"){}
	virtual bool test() override
	{
		_result = beTrue(3);
		if (!_result)
		{
			_failMessage = "beTrue(3)";
		}
		return _result;
	}
};

class FalseTest : public Test
{
public:
	FalseTest() : Test("beFalseTest"){}
	virtual bool test() override
	{
		_result = beFalse(5);
		if (!_result)
		{
			_failMessage = "beFalse(5)";
		}
		return _result;
	}
};

class EqualTest : public Test
{
public:
	EqualTest() : Test("beEqual"){}
	virtual bool test() override
	{
		_result = beEqual(10);
		if (!_result)
		{
			_failMessage = "beEqual(10)";
		}
		return _result;
	}
};

TESTHARNESS_API Test* getTests()
{
	Test* test1 = new TrueTest();
	Test* test2 = new FalseTest();
	Test* test3 = new EqualTest();
	test1->next(test2);
	test2->next(test3);
	return test1;
}
