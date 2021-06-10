#include "Message.h"
#include <ctime>

EndPoint::EndPoint(std::string addr, USHORT p) : address(addr), port(p) {}

std::string EndPoint::to_string() const
{
	return address + ":" + std::to_string(port);
}

EndPoint EndPoint::from_string(const std::string &str)
{
	EndPoint ep;
	size_t pos = str.find_first_of(':');
	if (pos != std::string::npos && pos < str.length())
	{
		ep.address = str.substr(0, pos);
		std::string portStr = str.substr(pos + 1);
		ep.port = std::stoi(portStr);
	}
	return ep;
}
