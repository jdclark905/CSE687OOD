/////////////////////////////////////////////////////////////////////
// Executive.cpp - directs execution of all program tasks
//

#include "Executive.h"
#include <iostream>
#include <functional>

const std::string globalFileSpec = "../Executive/Executive.h";

/*----< implicitly calls constructors for composed members >-------*/

Executive::Executive()
{
  std::cout << "\n  Executive starting up";
}
/*----< implicitly calls destructiors for composed members >-------*/

Executive::~Executive()
{
  std::cout << "\n  Executive shutting down";
}
/*----< directs execution of application >-------------------------*/

void Executive::doWork()
{
  t1.doWork();
  try
  {
    t2.doWork();  // throws
  }
  catch (std::exception& ex)
  {
    std::cout << "\n--" << ex.what();
  }

  // open file stream and read first four lines
  std::string fileName = globalFileSpec;
  std::ifstream strm = t3.doWork(fileName);
  if (strm.good())
  {
    std::string line;
    for (int i = 0; i < 4; ++i)
    {
      std::getline(strm, line);
      std::cout << "\n  " << line;
    }
  }
  else
  {
    std::cout << "\n  can't open file \"" << fileName << "\"";
  }

  // divide two doubles
  double num = 3.1415927;
  double denom = 2.0;
  std::cout << "\n  " << num << "/" << denom << " = " << t3.doWork(num, denom);
}
/*----< defines ATest's pure virtual test method >-----------------*/

bool Executive::test()
{
  bool tst1 = checkResult(t1.testPackage(), "Task1", "always passes");
  bool tst2 = checkResult(t2.testPackage(), "Task2", "always fails");
  bool tst3 = checkResult(t3.testPackage(), "Task3", "testing file access and arithmetic");

  std::string testFile = globalFileSpec;
  Task3 t3Test1(testFile, 3.1415927, 2);
  bool tst4 = checkResult(t3Test1.testPackage(), "Task3", "Expected to pass");

  Task3 t3Test2("doesn't exist", 3.1415927, 0);
  bool tst5 = checkResult(t3Test2.testPackage(), "Task3", "Expected to fail");

  return tst1 && tst2 && tst3 && tst4 && tst5;
}
/*----< private test application test class focused on test >------*/
/*
 *  often test class will do some test specific initialization
*/
class TestExecutive : public ATest
{
public:
  TestExecutive()
  {
    std::cout << "\n  Testing Executive";
    std::cout << "\n ===================";
  }
  /*----< invokes executive testPackage >--------------------------*/
  /*
   *  - Executive::testPackage() invokes the testPackage function
   *    for each composed member, e.g., Task1 and Task2.
   *  - This causes a chaining of tests for the entire application.
  */
  bool test()
  {
    Executive ex;
    return ex.testPackage();
  }
};
/////////////////////////////////////////////////////////////////////
// Cosmetic struct simply ensures that program output text
// ends with two newlines

struct Cosmetic
{
  ~Cosmetic() { std::cout << "\n\n"; }
};

/*----< application entry point >----------------------------------*/

int main(int argc, char* argv[])
{
  Cosmetic cos;  // first object constructed is last to be destroyed

  std::cout << "\n  Demonstrating Executive";
  std::cout << "\n =========================";
  Executive ex;
  ex.doWork();
  std::cout << "\n";

  TestExecutive te;
  if (te.test())
    std::cout << "\n--Executive and its minions passed their tests";
  else
    std::cout << "\n--Executive or at least one of its minions failed their tests";
}