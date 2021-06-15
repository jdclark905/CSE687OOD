class Comm
{
private:
	size_t _serverPort = 10000;
};



#include <vector>
#include <string>


// within DLL code, returns "testFunction1", "testFunction2"
std::vector<std::string> getTestFunctions()
{
	std::vector<std::string> names;
	names.push_back("testFunction1");
	names.push_back("testFunction2");
	return names;
}

// Tests function "a" in library
bool testFunction1()
{
	if (a() == true)
	{
		return true;
	}
	return false;
}

// Tests funciton "b" in library
bool testFunction2()
{
	if (b(3) == 3)
	{
		return true;
	}
	return false;
}

// Library functions
bool a();

int b(int _b);