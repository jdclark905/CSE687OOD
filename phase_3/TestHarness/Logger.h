#pragma once

#include <string>
#include <mutex>

#define TO_STR(x) std::to_string(x)

enum LogLevel {
	PassFailOnly,
	PassFailDetail,
};

class Logger
{
private:
	static std::mutex _coutMutex;
	static std::mutex _fileMutex;
	static std::string _fileName;
	static std::string _timestampFormat;
	static LogLevel _logLevel;

public:
	Logger();
	static void ToFile(const std::string &msg);
	static void ToConsole(const std::string &msg);
	static std::string CurrentTimeStamp();
};
