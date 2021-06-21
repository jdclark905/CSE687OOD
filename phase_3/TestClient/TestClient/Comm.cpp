#include "Comm.h"

/////////////////////////////////////////////////////////////////////////////
// Socket class
/////////////////////////////////////////////////////////////////////////////

Socket::Socket()
{

}

Socket::Socket(SOCKET sock) : _socket(sock)
{

}

Socket::Socket(Socket&& s)
{
	_socket = s._socket;
	s._socket = INVALID_SOCKET;
}

Socket& Socket::operator=(Socket&& s)
{
	if (this != &s)
	{
		_socket = s._socket;
		s._socket = INVALID_SOCKET;
	}
	return *this;
}

Socket::~Socket() {
	shutDown();
	close();
}

void Socket::close()
{
	if (_socket != INVALID_SOCKET)
		closesocket(_socket);
}

bool Socket::shutDownSend()
{
	shutdown(_socket, SD_SEND);
	if (_socket != INVALID_SOCKET)
		return true;
	return false;
}

bool Socket::shutDownRecv()
{
	shutdown(_socket, SD_RECEIVE);
	if (_socket != INVALID_SOCKET)
		return true;
	return false;
}

bool Socket::shutDown()
{
	shutdown(_socket, SD_BOTH);
	if (_socket != INVALID_SOCKET)
		return true;
	return false;
}

bool Socket::send(size_t bytes, byte* buffer)
{
	size_t bytesSent = 0, bytesLeft = bytes;
	byte* pBuf = buffer;
	while (bytesLeft > 0)
	{
		bytesSent = ::send(_socket, pBuf, bytesLeft, 0);
		if (_socket == INVALID_SOCKET || bytesSent == 0)
			return false;
		bytesLeft -= bytesSent;
		pBuf += bytesSent;
	}
	return true;
}

bool Socket::recv(size_t bytes, byte* buffer)
{
	size_t bytesRecvd = 0, bytesLeft = bytes;
	byte* pBuf = buffer;
	while (bytesLeft > 0)
	{
		bytesRecvd = ::recv(_socket, pBuf, bytesLeft, 0);
		if (_socket == INVALID_SOCKET || bytesRecvd == 0)
			return false;
		bytesLeft -= bytesRecvd;
		pBuf += bytesRecvd;
	}
	return true;
}

bool Socket::sendString(const std::string& str, byte terminator)
{
	size_t bytesSent, bytesRemaining = str.size();
	const byte* pBuf = &(*str.begin());
	while (bytesRemaining > 0)
	{
		bytesSent = ::send(_socket, pBuf, bytesRemaining, 0);
		if (bytesSent == INVALID_SOCKET || bytesSent == 0)
			return false;
		bytesRemaining -= bytesSent;
		pBuf += bytesSent;
	}
	::send(_socket, &terminator, 1, 0);
	return true;
}

std::string Socket::recvString(byte terminator)
{
	static const int buflen = 1;
	char buffer[1];
	std::string str;
	while (true)
	{
		iResult = ::recv(_socket, buffer, buflen, 0);
		if (iResult == 0 || iResult == INVALID_SOCKET)
		{
			break;
		}
		if (buffer[0] == terminator)
		{
			break;
		}
		str += buffer[0];
	}
	return str;
}

size_t Socket::sendStream(size_t bytes, byte* pBuf)
{
	return ::send(_socket, pBuf, bytes, 0);
}

size_t Socket::recvStream(size_t bytes, byte* pBuf)
{
	return ::recv(_socket, pBuf, bytes, 0);
}

size_t Socket::bytesWaiting()
{
	unsigned long int ret;
	::ioctlsocket(_socket, FIONREAD, &ret);
	return (size_t)ret;
}

bool Socket::waitForData(size_t timeToWait, size_t timeToCheck)
{
	size_t MaxCount = timeToWait / timeToCheck;
	static size_t count = 0;
	while (bytesWaiting() == 0)
	{
		if (++count < MaxCount)
			::Sleep(timeToCheck);
		else
			return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ClientSocket
/////////////////////////////////////////////////////////////////////////////

ClientSocket::ClientSocket() : Socket()
{

}

ClientSocket::ClientSocket(ClientSocket&& s) : Socket()
{
	_socket = s._socket;
	s._socket = INVALID_SOCKET;
}

ClientSocket& ClientSocket::operator=(ClientSocket&& s)
{
	if (this != &s)
	{
		_socket = s._socket;
		s._socket = INVALID_SOCKET;
	}
	return *this;
}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::connect(const std::string& ip, u_short port)
{
	// Create socket for connection
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET) {
		int error = WSAGetLastError();
		Logger::ToConsole("ClientSocket: create socket failed: " + std::to_string(error));
		return false;
	}

	Logger::ToConsole("ClientSocket: create socket successful");

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &saddr.sin_addr);

	// Connect socket
	iResult = ::connect(_socket, (sockaddr*)&saddr, sizeof(saddr));
	if (iResult == SOCKET_ERROR) {
		int error = WSAGetLastError();
		Logger::ToConsole("ClientSocket: connect socket failed: " + std::to_string(error));
		_socket = INVALID_SOCKET;
		return false;
	}

	Logger::ToConsole("ClientSocket: connect successful");
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// ServerSocket
/////////////////////////////////////////////////////////////////////////////

ServerSocket::ServerSocket(const std::string& ip, u_short port) : Socket(), _ip(ip), _port(port)
{
	Logger::ToConsole("ServerSocket starting up on " + ip + ":" + std::to_string(port));
}

ServerSocket::ServerSocket(ServerSocket&& sl) : Socket()
{
	_socket = sl._socket;
	sl._socket = INVALID_SOCKET;
}

ServerSocket& ServerSocket::operator=(ServerSocket&& s)
{
	if (this != &s)
	{
		_socket = s._socket;
		s._socket = INVALID_SOCKET;
	}
	return *this;
}

ServerSocket::~ServerSocket()
{

}

bool ServerSocket::bind()
{
	// Create a socket for listening
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET) {
		int error = WSAGetLastError();
		Logger::ToConsole("ServerSocket: create socket failed: " + std::to_string(error));
		return false;
	}

	Logger::ToConsole("ServerSocket: create socket successful");

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &saddr.sin_addr);

	// Bind socket to listening address
	iResult = ::bind(_socket, (sockaddr*)&saddr, sizeof(saddr));
	if (iResult == SOCKET_ERROR) {
		int error = WSAGetLastError();
		Logger::ToConsole("ServerSocket: bind listen socket failed: " + std::to_string(error));
		_socket = INVALID_SOCKET;
		return false;
	}

	Logger::ToConsole("ServerSocket: bind listen socket successful");
	return true;
}

bool ServerSocket::listen()
{
	iResult = ::listen(_socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		Logger::ToConsole("ServerSocket: listen failed");
		_socket = INVALID_SOCKET;
		return false;
	}
	Logger::ToConsole("ServerSocket: listen successful");
	return true;
}

// Called by listen thread to accept client connections
Socket ServerSocket::accept()
{
	struct sockaddr_in remaddr;
	socklen_t remaddr_len = sizeof(remaddr);
	std::string hostStr = "", portStr = "";
	SOCKET s = ::accept(_socket, (struct sockaddr*)&remaddr, &remaddr_len);
	Socket clientSocket = s;
	if (!clientSocket.validState())
	{
		int error = WSAGetLastError();
		Logger::ToConsole("ServerSocket accept failed: " + std::to_string(error));
		stop();
	}
	else
	{
		char host[16];
		ZeroMemory(host, sizeof(host));
		std::string ip = inet_ntop(AF_INET, &remaddr.sin_addr, host, 16);
		std::string remPort = std::to_string(ntohs(remaddr.sin_port));
		Logger::ToConsole("ServerSocket accepted connection from " + ip + ":" + remPort);
	}
	return clientSocket;
}

void ServerSocket::stop()
{
	_stop.exchange(true);
}
