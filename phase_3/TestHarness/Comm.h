#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <atomic>
#include <thread>
#include <unordered_map>
#include "BlockingQueue.h"
#include "Message.h"
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

	Socket(const std::string& ip = DEFAULT_LISTEN_IP, u_short port = DEFAULT_LISTEN_PORT);
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
	std::string _ip;
	u_short _port;
	int iResult;
};

class SocketListener : public Socket
{
public:
	// Constructor with port to listen on
	SocketListener(const std::string& ip = DEFAULT_LISTEN_IP, u_short port = DEFAULT_LISTEN_PORT);
	// Move constructor
	SocketListener(SocketListener&&);
	// Assignment operator
	SocketListener& operator=(SocketListener&&);
	// Destructor
	~SocketListener();

	template <typename CallableObject>
	bool start(CallableObject& co);
	void stop();
	BlockingQueue<Message>* responseQueue();

private:
	BlockingQueue<Message> _responseQueue;
	std::atomic<bool> _stop = false;
	bool bind();
	bool listen();
	Socket accept();
	
};

template <typename CallableObject>
bool SocketListener::start(CallableObject& co)
{
	if (!bind()) return false;
	if (!listen()) return false;

	// Listen on separate thread so it doesn't block main thread
	std::thread listenThread([&]()
	{
		Logger::ToConsole("SocketListener waiting for connections");
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
		Logger::ToConsole("SocketListener listen thread stopping");
	});
	listenThread.detach();
	return true;
}
