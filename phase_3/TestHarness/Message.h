#pragma once
#include <string>
#include <ctime>

#define MSG_TYPE_DEFAULT "DEFAULT"
#define MSG_TYPE_STATUS "STATUS"
#define MSG_TYPE_TEST_REQ "TEST_REQ"
#define MSG_TYPE_TEST_RESP "TEST_RESP"

struct Message
{
	std::string _source = "";
	std::string _dest = "";
	std::string _type = MSG_TYPE_DEFAULT;
	std::string _datetime = "";
	std::string _body = "";

	void setDateTimeNow()
	{
		_datetime = "now";
	}

	std::string describe()
	{
		std::string str = _datetime;
		str.append(" : Message from ").append(_source).append(" to ").append(_dest);
		str.append(" : ").append(_type).append(" : ").append(_body);
		return str;
	}
};