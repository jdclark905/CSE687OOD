/////////////////////////////////////////////////////////////////////
// Task2.cpp - second task for GettingStarted application
//
#include "Task3.h"
#include <iostream>
#include <fstream>
#include <sstream>

/*----< initialize Task3 instances >-------------------------------*/

Task3::Task3(const std::string& fileName, double num, double denom)
  : fileName_(fileName), num_(num), denom_(denom)
{
  std::cout << "\n  Starting Task3";
}
/*----< return resources used by Task3 instances >-----------------*/

Task3::~Task3()
{
  std::cout << "\n  Task3 shutting down";
}
/*----< execute Task3 activities >---------------------------------*/

std::ifstream Task3::doWork(const std::string& fileName)
{
  // c++ streams won't throw exceptions unless you tell them to do so, i.e.,
  // strm.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  std::ifstream strm;

  std::cout << "\n  performing Task #3 -- attempts to open file \"" << fileName.c_str() << "\"";

  strm.open(fileName);
  return strm;
}
/*----< execute another Task3 activity >---------------------------*/

double Task3::doWork(double num, double denom)
{
  std::cout << "\n  performing Task #3 -- divides " << num << " by " << denom;
  return (num / denom);
}
/*----< implement required self-test >-----------------------------*/

bool Task3::test()
{
  std::ostringstream out;

  double result = doWork(num_, denom_);  // divides num_ by denom_
  out << "result = " << result;
  bool tst1 = checkResult(result != INFINITY && result != NAN, "Task3::doWork(double, double)", out.str());

  std::ifstream strm = doWork(fileName_);
  bool tst2 = checkResult(strm.good(), "Task3:doWork(const std::string&)");

  return tst1 && tst2;
}
/*----< entry point simply invokes its own self-test >-------------*/

#ifdef TEST_TASK3

int main()
{
  std::cout << "\n  testing Task3";
  std::cout << "\n ---------------";

  Task3 t3("doesn't exist", 2.0, 3.0);
  t3.testPackage();
  std::cout << "\n\n";
}
#endif
