#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atomic>
#include <thread>
#include "Logger.h"
#include "Message.h"
#pragma comment (lib, "ws2_32.lib")

#define DEFAULT_PORT 10000

class Socket
{
public:
	using byte = char;

	// disable copy construction and assignment
	Socket(const Socket& s) = delete;
	Socket& operator=(const Socket& s) = delete;

	Socket();
	Socket(SOCKET);
	Socket(Socket&& s);
	operator SOCKET() { return _socket; }
	Socket& operator=(Socket&& s);
	virtual ~Socket();

	bool send(size_t bytes, byte* buffer);
	bool recv(size_t bytes, byte* buffer);
	size_t sendStream(size_t bytes, byte* buffer);
	size_t recvStream(size_t bytes, byte* buffer);
	bool sendString(const std::string& str, byte terminator = '\0');
	std::string recvString(byte terminator = '\0');
	static std::string removeTerminator(const std::string& src);
	size_t bytesWaiting();
	bool waitForData(size_t timeToWait, size_t timeToCheck);
	bool shutDownSend();
	bool shutDownRecv();
	bool shutDown();
	void close();
	bool validState() { return _socket != INVALID_SOCKET; }

protected:
	WSADATA _wsaData;
	SOCKET _socket;
	struct addrinfo *_result = NULL, *_ptr = NULL, _hints;
	int iResult;
};

class SocketListener : public Socket
{
public:
	// Constructor with port to listen on
	SocketListener(u_short port = DEFAULT_PORT);
	// Move constructor
	SocketListener(SocketListener&&);
	// Assignment operator
	SocketListener& operator=(SocketListener&&);
	// Destructor
	~SocketListener();

	bool start();
	void stop();

private:
	u_short _port;
	std::atomic<bool> _stop = false;

	bool bind();
	bool listen();
	Socket accept();
	
};
