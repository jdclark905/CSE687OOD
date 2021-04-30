#pragma once
/////////////////////////////////////////////////////////////////////
// Task3.h - third, slightly more typical, task for GettingStarted application
//
#include "../ATest/ATest.h"  // abstract base for this testable class
#include <fstream>
#include <string>

/////////////////////////////////////////////////////////////////////
// Task3 class - a top-level task

class Task3 : public ATest
{
public:
  Task3(const std::string& fileName = "", double num = 2.0, double denom = 3.0);
  virtual ~Task3();
  std::ifstream doWork(const std::string& fileName);
  double doWork(double num, double denom);
  virtual bool test();
private:
  std::string fileName_;
  double num_, denom_;
};
