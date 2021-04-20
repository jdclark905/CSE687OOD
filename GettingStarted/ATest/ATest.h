#pragma once
/////////////////////////////////////////////////////////////////////
// ATest.h - abstract base class for self testing classes
//
// Package operations:
// -------------------
// ATest is an abstract class that ensures all of its derived classes
// are safely testable, e.g., errors are properly handled and reported.
//
// - ATest.h declares a pure virtual function, test(), that every
//   derived class must implement, e.g., a self test.
//
// - It defines a non-virtual testPackage() function that every
//   derived class inherits.  The derived class runs its self test
//   indirectly by calling testPackage().  That runs test inside
//   a try-catch exception handler.
//
// - ATest also defines a non-virtual function, safeInvoke(...)
//   that allows a derived class to run a member function of any
//   other class with error handling.  The intent is that a testable
//   class can safely run methods on instances of testable classes it 
//   uses as part of its normal operation.
//
// - If you look at Executive.cpp you will see how these functions
//   are used.

#include <iostream>
#include <functional>

struct ATest
{
  /*---< non-virtual function inherited by all derived classes >---*/
  /*
   *  This function runs each of the derived classes' test()s
   *  inside a try-catch block, so we don't have to handle
   *  exceptions in those functions.
   */
  bool testPackage()
  {
    std::cout << "\n--entering " << typeid(*this).name() << "::testPackage()";
    try
    {
      return test();
    }
    catch (std::exception& ex)
    {
      std::cout << "\n--exception thrown in " << typeid(*this).name() << "::testPackage()";
      std::cout << "\n  " << ex.what();
      return false;
    }
  }
  /*----< display results of test >--------------------------------*/

  bool checkResult(bool result, const std::string& testName, const std::string& msg = "")
  {
    if (result)
    {
      std::cout << "\n  passed: " << testName.c_str();
      if (msg.length() > 0)
        std::cout << " --> " << msg.c_str();
      std::cout << "\n";
      return true;
    }
    else
    {
      std::cout << "\n  failed: " << testName.c_str();
      if (msg.length() > 0)
        std::cout << " --> " << msg.c_str();
      std::cout << "\n";
      return false;
    }
  }
  /*----< pure virtual function all derived classes implement >----*/

  virtual bool test()=0;
  
  /*----< virtual destructor redefined by each derived class >-----*/

  virtual ~ATest() {}
};