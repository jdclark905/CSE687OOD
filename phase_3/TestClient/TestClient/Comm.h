#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atomic>
#include <thread>
#include "BlockingQueue.h"
#include "Logger.h"
#pragma comment (lib, "ws2_32.lib")

#define DEFAULT_LISTEN_IP "127.0.0.1"
#define DEFAULT_LISTEN_PORT 10000

class Socket
{
public:
	using byte = char;

	// disable copy construction and assignment
	Socket(const Socket& s) = delete;
	Socket& operator=(const Socket& s) = delete;

	// Default constructor
	Socket();
	// Promotion constructor
	Socket(SOCKET);
	// Move constructor
	Socket(Socket&& s);
	// Conversion operator
	operator SOCKET() { return _socket; }
	// Move assignment operator
	Socket& operator=(Socket&& s);
	// Destructor
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
	int iResult;
};

class ClientSocket : public Socket
{
public:
	// Prevent copy construction and assignment
	ClientSocket(const ClientSocket&) = delete;
	ClientSocket& operator=(const ClientSocket&) = delete;

	ClientSocket();
	ClientSocket(ClientSocket&&);
	ClientSocket& operator=(ClientSocket&&);
	virtual ~ClientSocket();

	bool connect(const std::string& ip, u_short port);
};

class ServerSocket : public Socket
{
public:
	// Constructor with port to listen on
	ServerSocket(const std::string& ip = DEFAULT_LISTEN_IP, u_short port = DEFAULT_LISTEN_PORT);
	// Move constructor
	ServerSocket(ServerSocket&&);
	// Assignment operator
	ServerSocket& operator=(ServerSocket&&);
	// Destructor
	virtual ~ServerSocket();

	template <typename CallableObject>
	bool start(CallableObject& co);
	void stop();

private:
	std::atomic<bool> _stop = false;
	std::string _ip;
	u_short _port;
	bool bind();
	bool listen();
	Socket accept();

};

template <typename CallableObject>
bool ServerSocket::start(CallableObject& co)
{
	if (!bind()) return false;
	if (!listen()) return false;

	// Listen on separate thread so it doesn't block main thread
	std::thread listenThread([&]()
		{
			Logger::ToConsole("ServerSocket waiting for connections");
			while (!_stop.load())
			{
				// Accept client connection
				Socket clientSocket = accept();
				if (!clientSocket.validState())
				{
					continue;
				}

				// Start client handler in new thread
				std::thread clientThread(co, std::move(clientSocket));
				clientThread.detach();
			}
			Logger::ToConsole("ServerSocket listen thread stopping");
		});
	listenThread.detach();
	return true;
}
