#pragma once
#include <string>
#include <winsock.h>

class Message
{
public:
	
private:
	sockaddr_in _source;
	sockaddr_in _dest;
	std::string _type;
	std::string _author;
	std::string _datetime;
	std::string _body;
};