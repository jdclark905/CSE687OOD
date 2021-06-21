#pragma once

#include <string>
#include <unordered_map>
#include "Comm.h"

#define MSG_ATTR_NAME_FROM "from"			// endpoint of message source
#define MSG_ATTR_NAME_TO "to"				// endpoint of message destination
#define MSG_ATTR_NAME_TIMESTAMP "timestamp"	// timestamp of message
#define MSG_ATTR_NAME_AUTHOR "author"		// message author (client, test handler, etc)
#define MSG_ATTR_NAME_TYPE "type"			// message type (test request, test response, shutdown, etc)
#define MSG_ATTR_NAME_BODY "body"				// for test request/response, name of DLL

#define MSG_CHAR_DELIM ";"		// delimeter between attribute entries
#define MSG_CHAR_VAL_DELIM "="	// delimeter between attribute key and value
#define MSG_CHAR_TERM '\n'

#define MSG_TYPE_TEST_REQ "test request"
#define MSG_TYPE_TEST_RESP "test response"
#define MSG_TYPE_SHUTDOWN "shutdown"

struct MsgAddress
{
	std::string IPAddr;
	u_short Port;
	MsgAddress(std::string ipAddr = "", u_short port = 0);
	std::string toString();
	static MsgAddress fromString(const std::string& str);
};

inline MsgAddress::MsgAddress(std::string ipAddr, u_short port) : IPAddr(ipAddr), Port(port) {}

inline std::string MsgAddress::toString()
{
	return IPAddr + ":" + std::to_string(Port);
}

inline MsgAddress MsgAddress::fromString(const std::string& str)
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
	Socket* _clientSocket;

public:
	Message();
	Message(MsgAddress from, MsgAddress to);

	void setAttribute(const std::string& key, const std::string& value);
	std::string getValue(const std::string& key) const;
	bool hasKey(const std::string& key) const;
	std::string toString() const;
	static std::vector<std::string> splitAttributes(const std::string& src, const char* delim = MSG_CHAR_DELIM);
	static std::pair <std::string, std::string> splitKeyValue(const std::string&, const char* delim = MSG_CHAR_VAL_DELIM);
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
	void body(const std::string&);
	std::string body() const;
	void clientSocket(Socket*);
	Socket* clientSocket() const;
};

