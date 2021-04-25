#pragma once
/////////////////////////////////////////////////////////////////////
// Task1.h - second task for GettingStarted application
//
#include "../ATest/ATest.h"  // abstract base for this testable class
#include <iostream>

/////////////////////////////////////////////////////////////////////
// Task1 class - a top-level task

class Task1 : public ATest
{
public:
  Task1();
  virtual ~Task1();
  void doWork();
  virtual bool test();
};
