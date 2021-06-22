#include "pch.h"
//#include "stdafx.h"
#include "TestHarness.h"

std::vector<std::string> getTestNames()
{
	//gonna create a vector of strings
	std::vector<std::string> names;
	//then push all my function names to this vector
	names.push_back("beTrue");
	names.push_back("beFalse");
	names.push_back("beEqual");
	//return the vector
	return names;
}

bool beTrue(int x)
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

bool beFalse(int x)
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

bool beEqual(int x)
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

bool beTrueTest()
{
	return beTrue(3);
}

bool beFalseTest()
{
	return beFalse(5);
}

bool beEqualTest()
{
	return beEqual(10);
}