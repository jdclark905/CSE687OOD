#include "Logger.h"
#include <fstream>
#include <iostream>

bool Logger::ToConsole(std::string message)
{
	std::cout << message << std::endl;
	return true;
}