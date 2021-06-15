#include "Message.h"
#include "Logger.h"

Message::Message() {}

Message::Message(MsgAddress from, MsgAddress to)
{
	_attributes[MSG_ATTR_NAME_FROM] = from.to_string();
	_attributes[MSG_ATTR_NAME_TO] = to.to_string();
}

void Message::setAttribute(const std::string& key, const std::string& value)
{
	if (key != "")
	{
		_attributes[key] = value;
	}
}

std::string Message::getValue(const std::string& key) const
{
	std::string retVal = "";
	if (hasKey(key))
	{
		retVal = _attributes.at(key);
	}
	return retVal;
}

bool Message::hasKey(const std::string& key) const
{
	bool retVal = false;
	if (_attributes.find(key) != _attributes.end())
	{
		retVal = true;
	}
	return retVal;
}

std::string Message::toString() const
{
	std::string retVal;
	for (auto attrib : _attributes)
	{
		retVal += attrib.first + MSG_ATTR_VAL_DELIM + attrib.second + MSG_ATTR_DELIM;
	}
	return retVal;
}

std::vector<std::string> Message::splitAttributes(const std::string& src, const char* delim)
{
	std::vector<std::string> retVal;
	size_t start = 0;
	size_t found = src.find_first_of(delim);
	while ((found != std::string::npos) && (start < src.length()))
	{
		if (found > start)
		{
			retVal.push_back(src.substr(start, (found - start)));
		}
		start = found + 1;
		found = src.find_first_of(delim, start);
	}
	// The following block will only occur if the delimiter is missing from the end of the source
	if (start < src.length())
	{
		retVal.push_back(src.substr(start));
	}
	return retVal;
}

std::pair<std::string, std::string> Message::splitKeyValue(const std::string& line, const char* delim)
{
	std::pair<std::string, std::string> retVal = { "", "" };
	size_t found = line.find_first_of(delim);
	if (found != std::string::npos)
	{
		if (found > 0)
		{
			retVal.first = line.substr(0, found);
		}
		if (found < line.length() - 1)
		{
			retVal.second = line.substr(found + 1);
		}
	}
	return retVal;
}

Message Message::fromString(const std::string& msgStr)
{
	Message retVal;
	std::vector<std::string> attribs = splitAttributes(msgStr);
	for (std::string attribStr : attribs)
	{
		std::pair<std::string, std::string> attrib = splitKeyValue(attribStr);
		retVal.setAttribute(attrib.first, attrib.second);
	}
	return retVal;
}

void Message::from(MsgAddress addr)
{
	_attributes[MSG_ATTR_NAME_FROM] = addr.to_string();
}

MsgAddress Message::from() const
{
	MsgAddress retVal;
	if (hasKey(MSG_ATTR_NAME_FROM))
	{
		retVal = MsgAddress::from_string(_attributes.at(MSG_ATTR_NAME_FROM));
	}
	return retVal;
}

void Message::to(MsgAddress addr)
{
	_attributes[MSG_ATTR_NAME_TO] = addr.to_string();
}

MsgAddress Message::to() const
{
	MsgAddress retVal;
	if (hasKey(MSG_ATTR_NAME_TO))
	{
		retVal = MsgAddress::from_string(_attributes.at(MSG_ATTR_NAME_TO));
	}
	return retVal;
}

void Message::timestamp(const std::string& ts)
{
	_attributes[MSG_ATTR_NAME_TIMESTAMP] = ts;
}

std::string Message::timestamp() const
{
	std::string retVal = "";
	if (hasKey(MSG_ATTR_NAME_TIMESTAMP))
	{
		retVal = _attributes.at(MSG_ATTR_NAME_TIMESTAMP);
	}
	return retVal;
}

void Message::author(const std::string& auth)
{
	_attributes[MSG_ATTR_NAME_AUTHOR] = auth;
}

std::string Message::author() const
{
	std::string retVal = "";
	if (hasKey(MSG_ATTR_NAME_AUTHOR))
	{
		retVal = _attributes.at(MSG_ATTR_NAME_AUTHOR);
	}
	return retVal;
}

void Message::type(const std::string& msgType)
{
	_attributes[MSG_ATTR_NAME_TYPE] = msgType;
}

std::string Message::type() const
{
	std::string retVal = "";
	if (hasKey(MSG_ATTR_NAME_TYPE))
	{
		retVal = _attributes.at(MSG_ATTR_NAME_TYPE);
	}
	return retVal;
}

void Message::body(const std::string& body)
{
	_attributes[MSG_ATTR_NAME_BODY] = body;
}

std::string Message::body() const
{
	std::string retVal = "";
	if (hasKey(MSG_ATTR_NAME_BODY))
	{
		retVal = _attributes.at(MSG_ATTR_NAME_BODY);
	}
	return retVal;
}