#pragma once

#include <string>
#include <fstream>
#include <iostream>

using std::string;

enum LogLevel {
	PassFailOnly,
	PassFailMessage,
	PassFailMessageWithTimestamp
};

struct LogEntry
{
	LogLevel level = PassFailOnly;
	string message = "";
	bool status;
	string error = "";

};

class Logger
{

public:
	//default constructor
	Logger();
	bool ToFile(std::string);
	bool ToConsole(std::string);
};
