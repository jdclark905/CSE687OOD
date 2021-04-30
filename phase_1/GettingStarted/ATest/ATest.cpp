// ATest.cpp

#include "ATest.h"

#ifdef TEST_ATEST
#include <iostream>

class Tester : public ATest
{
  bool test() { return true; }
};

int main()
{
  Tester tester;
  tester.testPackage();
}
#endif
