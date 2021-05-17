#include "UnitTest.h"

UnitTest::UnitTest() {}

// the assertion stuct is passed in to the assert function
void UnitTest::AssertEquals(
    int (*function)(),
    int test,
    assertion assert
)
{
    if ((*function)() == test)
    {
        assert.result = true;
    }
    else
    {
        assert.result = false;
    }
}

/*
vector<assertion> UnitTest::GetAssertions()
{
    return asserts_;
}
*/

int testFunction1()
{
    // test passed
    return 0;
}

int testFunction2()
{
    // test failed
    return -1;
}

int testFunction3()
{
    // exception raised
    std::string("abc").substr(10);
    return 0;
}
