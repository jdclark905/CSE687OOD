#include "Logger.h"

Logger::Logger()
{

}

bool Logger::ToConsole(std::string message)
{
	std::cout << message << std::endl;
	return true;
}

bool Logger::ToFile(std::string message)
{
	//open file in write mode
	std::ofstream outfile;
	outfile.open("Harness_Logger.txt", std::ios::app); //this will append to the file

	//the actual writing
	outfile << message << std::endl;

	outfile.close();

	return true;
}
