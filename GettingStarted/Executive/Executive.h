#pragma once
/////////////////////////////////////////////////////////////////////
// Executive.h - directs execution of all program tasks
//
#include "../ATest/ATest.h"  // abstract base for all testable classes
#include "../Task1/Task1.h"  // a testable application task
#include "../Task2/Task2.h"  // ditto
#include "../Task3/Task3.h"  // yada yada

class Executive : public ATest
{
public:
  Executive();
  ~Executive();
  void doWork();
  bool test();
private:
  Task1 t1;
  Task2 t2;
  Task3 t3;
};