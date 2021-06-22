// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "MathLibrary.h"
#include <vector>
#include <string>

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
bool mathFunc1()
{
    int a = 0;
    int b = 1;

    // do some other calculations

    if (a > b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool mathFunc2()
{
    int a = 1;
    int b = 0;

    // do some other calculations

    if (a > b)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool mathFunc3()
{
    std::string("3.14").substr(10);
    return false;
}

std::vector<std::string> getTestNames()
{
    std::vector<std::string> names;
    names.push_back("mathFunc1");
    names.push_back("mathFunc2");
    names.push_back("mathFunc3");
    return names;
}