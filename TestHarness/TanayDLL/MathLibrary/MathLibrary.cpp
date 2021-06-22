// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "MathLibrary.h"

// DLL internal state variables:
static unsigned long long previous_;  // Previous value, if any
static unsigned long long current_;   // Current sequence value
static unsigned index_;               // Current seq. position

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
void fibonacci_init(
    const unsigned long long a,
    const unsigned long long b)
{
    index_ = 0;
    current_ = a;
    previous_ = b; // see special case when initialized
}

// Produce the next value in the sequence.
// Returns true on success, false on overflow.
bool fibonacci_next()
{
    // check to see if we'd overflow result or position
    if ((ULLONG_MAX - previous_ < current_) ||
        (UINT_MAX == index_))
    {
        return false;
    }

    // Special case when index == 0, just return b value
    if (index_ > 0)
    {
        // otherwise, calculate next sequence value
        previous_ += current_;
    }
    std::swap(current_, previous_);
    ++index_;
    return true;
}

// Get the current value in the sequence.
unsigned long long fibonacci_current()
{
    return current_;
}

// Get the current index position in the sequence.
unsigned fibonacci_index()
{
    return index_;
}


// CUSTOM FUNCTIONS

class MathFunc1 : public Test
{
public:
	MathFunc1() : Test("MathFunc1"){}
	virtual bool test() override
	{
		int a = 0;
		int b = 1;

		// do some other calculations

		if (a > b)
		{
			_result = true;
		}
		else
		{
			_result = false;
			_failMessage = "a < b, a == " + TO_STR(a) + ", b == " + TO_STR(b);
		}
		return _result;
	}
};

class MathFunc2 : public Test
{
public:
	MathFunc2() : Test("MathFunc2"){}
	virtual bool test() override
	{
		int a = 1;
		int b = 0;

		// do some other calculations

		if (a > b)
		{
			_result = true;
		}
		else
		{
			_result = false;
			_failMessage = "a < b, a == " + TO_STR(a) + ", b == " + TO_STR(b);
		}
		return _result;
	}
};

class MathFunc3 : public Test
{
public:
	MathFunc3() : Test("MathFunc3"){}
	virtual bool test() override
	{
		std::string("3.14").substr(10);
		return false;
	}
};

MATHLIBRARY_API Test* getTests()
{
	Test* test1 = new MathFunc1();
	Test* test2 = new MathFunc2();
	Test* test3 = new MathFunc3();
	test1->next(test2);
	test2->next(test3);
	return test1;
}