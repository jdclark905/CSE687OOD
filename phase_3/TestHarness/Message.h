#pragma once

#include <string>
#include <map>
#include <ctime>
#include "Logger.h"

enum MessageType
{
	Other,
	RunnerReady,
	RunnerTesting,
	TestResult
};

std::map<MessageType, std::string> MessageTypeMap = 
{
	{ Other, "Other" },
	{ RunnerReady, "Ready" },
	{ RunnerTesting, "Testing" },
	{ TestResult, "Test Result" }
};

struct Message
{
	std::string source = "";
	MessageType type = Other;
	std::string timestamp = Logger::CurrentTimeStamp();
	std::string body = "";
};