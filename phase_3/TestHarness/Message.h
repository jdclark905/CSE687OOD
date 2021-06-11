#pragma once

#include <string>
#include <unordered_map>
#include <winsock2.h>
#include "BlockingQueue.h"

class Message
{
private:
	std::unordered_map<std::string, std::string> _attributes;

public:
	Message();
	Message(PSOCKADDR_IN from, PSOCKADDR_IN to);
	void from(PSOCKADDR_IN from);
	SOCKADDR_IN from() const;
	void to(PSOCKADDR_IN to);
	SOCKADDR_IN to() const;
	void command(const std::string& cmd);
	std::string command() const;
};

