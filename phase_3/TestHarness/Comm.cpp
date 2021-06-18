#include "Comm.h"

/////////////////////////////////////////////////////////////////////////////
// Socket class members

//----< constructor sets TCP protocol and Stream mode >----------------------
Socket::Socket()
{
	ZeroMemory(&_hints, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_protocol = IPPROTO_TCP;
}

//----< promotes Win32 socket to Socket >------------------------------------
Socket::Socket(SOCKET sock) : Socket()
{
	_socket = sock;
}

//----< transfer socket ownership with move constructor >--------------------
Socket::Socket(Socket&& s)
{
	_socket = s._socket;
	s._socket = INVALID_SOCKET;
	_hints = s._hints;
}

//----< transfer socket ownership with move assignment >---------------------
Socket& Socket::operator=(Socket&& s)
{
	if (this == &s) return *this;
	_socket = s._socket;
	s._socket = INVALID_SOCKET;
	_hints = s._hints;
	return *this;
}

//----< close connection >---------------------------------------------------
void Socket::close()
{
	if (_socket != INVALID_SOCKET)
		closesocket(_socket);
}

//----< tells receiver there will be no more sends from this socket >--------
bool Socket::shutDownSend()
{
	shutdown(_socket, SD_SEND);
	if (_socket != INVALID_SOCKET)
		return true;
	return false;
}

//----< tells receiver this socket won't call receive anymore >--------------
bool Socket::shutDownRecv()
{
	shutdown(_socket, SD_RECEIVE);
	if (_socket != INVALID_SOCKET)
		return true;
	return false;
}

//----< tells receiver there will be no more sends or recvs >----------------
bool Socket::shutDown()
{
	shutdown(_socket, SD_BOTH);
	if (_socket != INVALID_SOCKET)
		return true;
	return false;

}

//----< destructor closes socket handle >------------------------------------
Socket::~Socket() {
	shutDown();
	close();
}

//----< send buffer >--------------------------------------------------------
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

//----< recv buffer >--------------------------------------------------------
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

//----< sends a terminator terminated string >-------------------------------
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

//----< receives terminator terminated string >------------------------------
std::string Socket::recvString(byte terminator)
{
	static const int buflen = 1;
	char buffer[1];
	std::string str;
	bool first = true;
	while (true)
	{
		iResult = ::recv(_socket, buffer, buflen, 0);
		if (iResult == 0 || iResult == INVALID_SOCKET)
		{
			//StaticLogger<1>::write("\n  -- invalid socket in Socket::recvString");
			break;
		}
		if (buffer[0] == terminator)
		{
			// added 9/29/2017
			str += terminator;
			break;
		}
		str += buffer[0];
	}
	return str;
}

//----< strips terminator character that recvString includes >---------------
std::string Socket::removeTerminator(const std::string& src)
{
	return src.substr(0, src.size() - 1);
}

//----< attempt to send specified number of bytes, but may not send all >----
size_t Socket::sendStream(size_t bytes, byte* pBuf)
{
	return ::send(_socket, pBuf, bytes, 0);
}

//----< attempt to recv specified number of bytes, but may not send all >----
size_t Socket::recvStream(size_t bytes, byte* pBuf)
{
	return ::recv(_socket, pBuf, bytes, 0);
}

//----< returns bytes available in recv buffer >-----------------------------
size_t Socket::bytesWaiting()
{
	unsigned long int ret;
	::ioctlsocket(_socket, FIONREAD, &ret);
	return (size_t)ret;
}

//----< waits for server data, checking every timeToCheck millisec >---------
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



///////////////////////////////////////
// SocketListener

SocketListener::SocketListener(u_short port) : _port(port)
{
	_socket = INVALID_SOCKET;
	ZeroMemory(&_hints, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_protocol = IPPROTO_TCP;
	_hints.ai_flags = AI_PASSIVE;
	Logger::ToConsole("SocketListener created on port " + std::to_string(port));
}

SocketListener::SocketListener(SocketListener&& sl)
{
	_socket = sl._socket;
	sl._socket = INVALID_SOCKET;
	_hints = sl._hints;
}

SocketListener& SocketListener::operator=(SocketListener&& sl)
{
	if (this != &sl)
	{
		_socket = sl._socket;
		sl._socket = INVALID_SOCKET;
		_hints = sl._hints;
	}
	return *this;
}

SocketListener::~SocketListener()
{

}

bool SocketListener::bind()
{
	Logger::ToConsole("SocketListener binding");
	iResult = getaddrinfo(NULL, std::to_string(_port).c_str(), &_hints, &_result);
	if (iResult != 0)
	{
		Logger::ToConsole("SocketListener.start() getaddrinfo failure: " + std::to_string(iResult));
		return false;
	}

	// Iterate through results to create and bind listening socket
	for (auto pResult = _result; pResult != NULL; pResult = pResult->ai_next)
	{
		// Create a SOCKET for connecting to server
		_socket = socket(pResult->ai_family, pResult->ai_socktype, pResult->ai_protocol);
		if (_socket == INVALID_SOCKET) {
			int error = WSAGetLastError();
			Logger::ToConsole("SocketListener.start() failed to create socket: " + std::to_string(error));
			continue;
		}
		Logger::ToConsole("SocketListener.start() created listening socket");

		// Setup the TCP listening socket
		iResult = ::bind(_socket, pResult->ai_addr, (int)pResult->ai_addrlen);
		sockaddr_in* saddr = (sockaddr_in*)pResult->ai_addr;
		Logger::ToConsole("Socket address: " + std::to_string(saddr->sin_addr.S_un.S_addr) + ":" + std::to_string(saddr->sin_port));
		if (iResult == SOCKET_ERROR) {
			int error = WSAGetLastError();
			Logger::ToConsole("SocketListener.start() failed to bind socket: " + std::to_string(error));
			_socket = INVALID_SOCKET;
			continue;
		}
		else
		{
			break;  // bind to first available
			//continue;   // bind to all
		}
	}
	freeaddrinfo(_result);
	if (_socket != INVALID_SOCKET)
	{
		Logger::ToConsole("SocketListener.start() socket bind successful");
		return true;
	}
	return false;
}

bool SocketListener::listen()
{
	Logger::ToConsole("SocketListener starting listening");
	iResult = ::listen(_socket, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		Logger::ToConsole("SocketListener.listen() listen failed");
		_socket = INVALID_SOCKET;
		return false;
	}
	Logger::ToConsole("SocketListener.listen() listen successful");
	return true;
}

Socket SocketListener::accept()
{
	SOCKET s = ::accept(_socket, NULL, NULL);
	Socket clientSocket = s;
	if (!clientSocket.validState())
	{
		int error = WSAGetLastError();
		Logger::ToConsole("SocketListener.accept() failed: " + std::to_string(error));
	}
	return clientSocket;
}

bool SocketListener::start()
{
	if (!bind()) return false;
	if (!listen()) return false;

	std::thread listenThread([&]()
	{
		Logger::ToConsole("SocketListener waiting for connections");
		while (!_stop.load())
		{
			Socket clientSocket = accept();
			if (!clientSocket.validState())
			{
				continue;
			}
			Logger::ToConsole("SocketListener accepted connection");
		}
	});
	listenThread.detach();
	return true;
}

void SocketListener::stop()
{
	_stop.exchange(true);
}