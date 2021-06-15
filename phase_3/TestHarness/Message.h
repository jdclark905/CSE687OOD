#pragma once

#include <string>
#include <unordered_map>

#define MSG_ATTR_FROM "from"
#define MSG_ATTR_TO "to"
#define MSG_ATTR_TIMESTAMP "timestamp"
#define MSG_ATTR_AUTHOR "author"
#define MSG_ATTR_TYPE "type"
#define MSG_ATTR_DELIM ";"		// delimeter between attribute entries
#define MSG_ATTR_VAL_DELIM "="	// delimeter between attribute key and value

struct MsgAddress
{
	std::string IPAddr;
	size_t Port;
	MsgAddress(std::string ipAddr = "", size_t port = 0);
	std::string to_string();
	static MsgAddress from_string(const std::string& str);
};

inline MsgAddress::MsgAddress(std::string ipAddr, size_t port) : IPAddr(ipAddr), Port(port) {}

inline std::string MsgAddress::to_string()
{
	return IPAddr + ":" + std::to_string(Port);
}

inline MsgAddress MsgAddress::from_string(const std::string& str)
{
	MsgAddress msgAddr;
	size_t found = str.find_first_of(":");
	if (found != std::string::npos)
	{
		if (found > 0)
		{
			msgAddr.IPAddr = str.substr(0, found);
		}
		if (found < (str.length() - 1))
		{
			msgAddr.Port = std::stoi(str.substr(found + 1));
		}
	}
	return msgAddr;
}

// Class for handling messages
class Message
{
private:
	std::unordered_map<std::string, std::string> _attributes;

public:
	Message();
	Message(MsgAddress from, MsgAddress to);

	void setAttribute(const std::string& key, const std::string& value);
	bool hasKey(const std::string& key) const;
	std::string toString() const;
	static std::vector<std::string> splitAttributes(const std::string& src, const char* delim = MSG_ATTR_DELIM);
	static std::pair <std::string, std::string> getKeyValue(const std::string&, const char* delim = MSG_ATTR_VAL_DELIM);
	static Message fromString(const std::string&);

	void from(MsgAddress);
	MsgAddress from() const;
	void to(MsgAddress);
	MsgAddress to() const;
	void timestamp(const std::string&);
	std::string timestamp() const;
	void author(const std::string&);
	std::string author() const;
	void type(const std::string&);
	std::string type() const;
};

