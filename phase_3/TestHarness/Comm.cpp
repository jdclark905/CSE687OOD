#include "Comm.h"

///////////////////////////////////////
// SocketListener

ConnectionListener::ConnectionListener(u_short port) : _port(port)
{
	_socket = INVALID_SOCKET;
	ZeroMemory(&_hints, sizeof(_hints));
	_hints.ai_family = AF_INET;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_protocol = IPPROTO_TCP;
	_hints.ai_flags = AI_PASSIVE;
}

ConnectionListener::ConnectionListener(ConnectionListener&& sl)
{
	_socket = sl._socket;
	sl._socket = INVALID_SOCKET;
	_hints = sl._hints;
}

ConnectionListener& ConnectionListener::operator=(ConnectionListener&& sl)
{
	if (this == &sl) return *this;
	_socket = sl._socket;
	sl._socket = INVALID_SOCKET;
	_hints = sl._hints;
}

ConnectionListener::~ConnectionListener()
{

}

bool ConnectionListener::bind()
{
	int result = getaddrinfo(NULL, std::to_string(_port).c_str(), &_hints, &_result);
	if (!result)
	{
		Logger::ToConsole("ConnectionListener.start() getaddrinfo failure: " + std::to_string(result));
		return false;
	}

	// Iterate through results to create and bind listening socket
	for (auto pResult = _result; pResult != NULL; pResult = pResult->ai_next)
	{
		// Create a SOCKET for connecting to server
		_socket = socket(pResult->ai_family, pResult->ai_socktype, pResult->ai_protocol);
		if (_socket == INVALID_SOCKET) {
			int error = WSAGetLastError();
			Logger::ToConsole("ConnectionListener.start() failed to create socket: " + std::to_string(error));
			continue;
		}
		Logger::ToConsole("ConnectionListener.start() created listening socket");

		// Setup the TCP listening socket
		result = ::bind(_socket, pResult->ai_addr, (int)pResult->ai_addrlen);
		sockaddr_in* saddr = (sockaddr_in*)pResult->ai_addr;
		Logger::ToConsole("Socket address: " + std::to_string(saddr->sin_addr.S_un.S_addr) + ":" + std::to_string(saddr->sin_port));
		if (result == SOCKET_ERROR) {
			int error = WSAGetLastError();
			Logger::ToConsole("ConnectionListener.start() failed to bind socket: " + std::to_string(error));
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
		Logger::ToConsole("ConnectionListener.start() socket bind successful");
		return true;
	}
	return false;
}

bool ConnectionListener::listen()
{
	int result = ::listen(_socket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		int error = WSAGetLastError();
		Logger::ToConsole("ConnectionListener.listen() listen failed");
		_socket = INVALID_SOCKET;
		return false;
	}
	Logger::ToConsole("ConnectionListener.listen() listen successful");
	return true;
}