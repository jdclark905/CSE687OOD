#pragma once
/////////////////////////////////////////////////////////////////////
// Task2.h - second task for GettingStarted application
//
#include "../ATest/ATest.h"  // abstract base for this testable class
#include <iostream>

/////////////////////////////////////////////////////////////////////
// Task2 class - a top-level task

class Task2 : public ATest
{
public:
  Task2();
  virtual ~Task2();
  void doWork();
  virtual bool test();
};
