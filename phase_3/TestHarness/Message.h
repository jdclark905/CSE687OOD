#pragma once

#include <string>
#include <unordered_map>
#include <winsock.h>

struct EndPoint
{
	std::string address;
	int port;
	
	EndPoint(std::string addr = "localhost", USHORT p = 10000);
	std::string to_string() const;
	static EndPoint from_string(const std::string& str);
};

class Message
{
private:
	std::unordered_map<std::string, std::string> _attributes;
	struct sockaddr_in sa;
public:
	Message();
	Message(EndPoint src, EndPoint dst);
};
