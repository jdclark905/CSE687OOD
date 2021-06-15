#pragma once

#include <string>
#include <mutex>

enum LogLevel {
	PassFailOnly,
	PassFailMessage,
	PassFailMessageWithTimestamp
};

struct TestResult
{
	LogLevel level = PassFailOnly;
	std::string message = "";
	bool pass;
	std::string error = "";
};

class Logger
{
private:
	static std::mutex _coutMutex;
	static std::mutex _fileMutex;
	static std::string _fileName;
	static std::string _timestampFormat;

public:
	Logger();
	static void ToFile(const std::string &msg);
	static void ToConsole(const std::string &msg);
	static std::string CurrentTimeStamp();
};
