#include "UnitTest.h"
using std::_Xruntime_error;

UnitTest::UnitTest() {}

// the assertion stuct is passed in to the assert function
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
    // raise exception
    //std::string("abc").substr(10);
    return 0;
}
