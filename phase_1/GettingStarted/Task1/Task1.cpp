/////////////////////////////////////////////////////////////////////
// Task1.cpp - first task for GettingStarted application
//
#include "Task1.h"

/*----< initialize Task1 instances >-------------------------------*/

Task1::Task1()
{
  std::cout << "\n  Starting Task1";
}
/*----< return resources used by Task1 instances >-----------------*/

Task1::~Task1()
{
  std::cout << "\n  Task1 shutting down";
}
/*----< execute Task1 activities >---------------------------------*/

void Task1::doWork()
{
  std::cout << "\n  performing Task #1 -- trivial";
}
/*----< implement required self-test >-----------------------------*/

bool Task1::test()
{
  doWork();
  return true;
}

/*----< entry point simply invokes its own self-test >-------------*/

#ifdef TEST_TASK1

int main()
{
  std::cout << "\n  testing Task1";
  std::cout << "\n ---------------";

  Task1 t1;
  t1.testPackage();
  std::cout << "\n\n";
}
#endif
