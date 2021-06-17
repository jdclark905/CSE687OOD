#pragma once

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Logger.h"

#define DEFAULT_PORT 10000

class ConnectionListener
{
public:
	// Constructor with port to listen on
	ConnectionListener(u_short port = DEFAULT_PORT);
	// Move constructor
	ConnectionListener(ConnectionListener&&);
	// Assignment operator
	ConnectionListener& operator=(ConnectionListener&&);
	// Destructor
	~ConnectionListener();

	template <typename CallableObject>
	bool start(CallableObject& co);
	void stop();

private:
	SOCKET _socket;
	u_short _port;
	struct addrinfo *_result = nullptr, *_ptr = nullptr, _hints;

	bool bind();
	bool listen();
	void listenThread();
};

class Comm
{
public:

private:
	
};

template <typename CallableObject>
bool ConnectionListener::start(CallableObject& co)
{
	if (!bind())
	{
		return false;
	}

	if (!listen())
	{
		return false;
	}

	return true;
}