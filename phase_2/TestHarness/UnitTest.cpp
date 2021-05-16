#include "UnitTest.h"

UnitTest::UnitTest() {}

// copies of int test, and assertion stuct are passed in the assert function
void UnitTest::AssertEquals(
    int (*function)(),
    int test, 
    assertion assert,
    string func_name
)
{
    assert.assertion_type = "equals";
    assert.function_name= func_name;

    if ((*function)() == test)
    {
        assert.result = true;
    }
    else
    {
        assert.result = false;
    }

    // store assertion struct in vector of assertion structs
    asserts_.push_back(assert);
}

vector<assertion> UnitTest::GetAssertions()
{
    return asserts_;
}

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
    // raises exception
    int number = 10/5;

    return number;
}
