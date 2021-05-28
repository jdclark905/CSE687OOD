#include <string>

class ITest
{
public:
	// Implements to run test(s)
	virtual bool execute() = 0;

	bool runTest(bool (*function)());
};