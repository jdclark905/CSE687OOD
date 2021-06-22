#include "UnitTest.h"

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

