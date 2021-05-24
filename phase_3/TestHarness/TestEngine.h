#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include "UnitTest.h"
#include "Logger.h"

class TestEngine
{
private:
	TestEngine();
	static TestEngine instance;
	std::string FormatMessage(assertion);
	std::string GetTimestamp();

public:
	TestEngine(const TestEngine&) = delete;
	TestEngine operator=(const TestEngine&) = delete;
	static TestEngine& GetInstance();

	void Execute();
};
