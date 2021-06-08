#include <chrono>
#include <iostream>
#include <fstream>
#include "Logger.h"

std::mutex Logger::_coutMutex;
std::mutex Logger::_fileMutex;
std::string Logger::_fileName = "Harness_Logger.txt";

// Print to console
void Logger::ToConsole(const std::string &msg)
{
	std::lock_guard<std::mutex> lock(_coutMutex);
	std::cout << msg << '\n';
}

// Append log file
void Logger::ToFile(const std::string &msg)
{
	std::ofstream logFile;
	logFile.open(_fileName.c_str(), std::ios::app);
	logFile << msg << '\n';
	logFile.close();
}

// Get current timestamp, formatted as 'MM/DD/YY HH:MM:SS'
std::string Logger::CurrentTimeStamp()
{
	time_t now;
	struct tm timeinfo;
	char buffer[20];

	time(&now);
	localtime_s(&timeinfo, &now);

	strftime(buffer, 20, "%D %T", &timeinfo);
	return (std::string)buffer;
}