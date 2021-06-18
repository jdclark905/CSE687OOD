#include "Comm.h"

/////////////////////////////////////////////////////////////////////////////
// Socket class
/////////////////////////////////////////////////////////////////////////////

Socket::Socket(const std::string& ip, u_short port) : _ip(ip), _port(port)
{
	_socket = INVALID_SOCKET;

}

Socket::Socket(SOCKET sock) : Socket()
{
	_socket = sock;
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
// SocketListener
/////////////////////////////////////////////////////////////////////////////

SocketListener::SocketListener(const std::string& ip, u_short port) : Socket(ip, port)
{
	Logger::ToConsole("SocketListener created on port " + std::to_string(port));
}

SocketListener::SocketListener(SocketListener&& sl)
{
	_socket = sl._socket;
	sl._socket = INVALID_SOCKET;
}

SocketListener& SocketListener::operator=(SocketListener&& sl)
{
	if (this != &sl)
	{
		_socket = sl._socket;
		sl._socket = INVALID_SOCKET;
	}
	return *this;
}

SocketListener::~SocketListener()
{

}

bool SocketListener::bind()
{
	Logger::ToConsole("SocketListener binding");

	// Create a SOCKET for connecting to server
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET) {
		int error = WSAGetLastError();
		Logger::ToConsole("SocketListener failed to create listen socket: " + std::to_string(error));
	}
	Logger::ToConsole("SocketListener created listen socket");

	struct sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(_port);
	inet_pton(AF_INET,_ip.c_str(), &saddr.sin_addr);

	// Bind socket to listening address
	iResult = ::bind(_socket, (sockaddr*)&saddr, sizeof(saddr));
	if (iResult == SOCKET_ERROR) {
		int error = WSAGetLastError();
		Logger::ToConsole("SocketListener failed to bind socket: " + std::to_string(error));
		_socket = INVALID_SOCKET;
	}
	if (_socket != INVALID_SOCKET)
	{
		Logger::ToConsole("SocketListener listen socket bound to " + _ip + ":" + std::to_string(_port));
		return true;
	}
	return false;
}

bool SocketListener::listen()
{
	iResult = ::listen(_socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		Logger::ToConsole("SocketListener listen failed");
		_socket = INVALID_SOCKET;
		return false;
	}
	Logger::ToConsole("SocketListener listen successful");
	return true;
}

// Called by listen thread to accept client connections
Socket SocketListener::accept()
{
	struct sockaddr_in remaddr;
	socklen_t remaddr_len = sizeof(remaddr);
	char host[NI_MAXHOST];
	char port[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(port, NI_MAXSERV);
	std::string hostStr = "", portStr = "";
	SOCKET s = ::accept(_socket, (struct sockaddr*)&remaddr, &remaddr_len);
	Socket clientSocket = s;
	if (!clientSocket.validState())
	{
		int error = WSAGetLastError();
		Logger::ToConsole("SocketListener accept failed: " + std::to_string(error));
		stop();
	}
	else
	{
		if (getnameinfo((sockaddr*)&remaddr, remaddr_len, host, NI_MAXHOST, port, NI_MAXSERV, 0) == 0)
		{
			hostStr.assign(host);
			portStr.assign(port);
		}
		else
		{

		}
		Logger::ToConsole("SocketListener accepted connection from " + hostStr + ":" + portStr);
	}
	return clientSocket;
}

void SocketListener::stop()
{
	_stop.exchange(true);
}