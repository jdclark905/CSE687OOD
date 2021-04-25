/////////////////////////////////////////////////////////////////////
// Task2.cpp - second task for GettingStarted application
//
#include "Task2.h"

/*----< initialize Task2 instances >-------------------------------*/

Task2::Task2()
{
  std::cout << "\n  Starting Task2";
}
/*----< return resources used by Task2 instances >-----------------*/

Task2::~Task2()
{
  std::cout << "\n  Task2 shutting down";
}
/*----< execute Task2 activities >---------------------------------*/

void Task2::doWork()
{
  std::cout << "\n  performing Task #2 -- throws exeception";
  throw std::exception("something bad happend in Task2::doWork()");
}
/*----< implement required self-test >-----------------------------*/

bool Task2::test()
{
  doWork();
  return true;
}
/*----< entry point simply invokes its own self-test >-------------*/

#ifdef TEST_TASK2

int main()
{
  std::cout << "\n  testing Task2";
  std::cout << "\n ---------------";

  Task2 t2;
  t2.testPackage();
  std::cout << "\n\n";
}
#endif
