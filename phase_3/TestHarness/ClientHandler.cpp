#include "ClientHandler.h"

ClientHandler::ClientHandler(BlockingQueue<Message>& requestQueue, BlockingQueue<Message>& responseQueue) : _requestQueue(requestQueue), _responseQueue(responseQueue)
{

}

void ClientHandler::start()
{
	// define callable object to handle client connections
	// accepted by listener thread
	MsgReceiver* rcvr = new MsgReceiver(_requestQueue);

	// Start listener
	_listener.start(*rcvr);
	_responseThread = new std::thread(&ClientHandler::responder, this);
}

void ClientHandler::shutdown()
{
	Message msgShutdown;
	msgShutdown.type(MSG_TYPE_SHUTDOWN);
	_responseQueue.enqueue(msgShutdown);
	_responseThread->join();
}

/////////////////////////////////////////
// Message Receiver Class
/////////////////////////////////////////

ClientHandler::MsgReceiver::MsgReceiver(BlockingQueue<Message>& requestQueue) : _requestQueue(requestQueue)
{

}

void ClientHandler::MsgReceiver::operator()(Socket& s)
{
	Logger::ToConsole("ClientHandler receiver started");
	while (s.validState())
	{
		// Receive message (without terminator)
		std::string msgStr = s.recvString();
		if (msgStr.length() == 0)
		{
			break;	// Invalid message string
		}
		Message msg = Message::fromString(msgStr);
		msg.clientSocket(&s);
		Logger::ToConsole("ClientHandler received message: " + msg.toString());
		_requestQueue.enqueue(msg);
		if (msg.type() == MSG_TYPE_SHUTDOWN)
		{
			break;
		}
	}
	Logger::ToConsole("ClientHandler terminating");
}

void ClientHandler::responder()
{
	Logger::ToConsole("Response Handler responder started");
	while (true)
	{
			Message msg = _responseQueue.dequeue();
			Logger::ToConsole("Response Handler received message: " + msg.toString());
			if (msg.type() == MSG_TYPE_SHUTDOWN)
			{
				break;
			}
	}
	Logger::ToConsole("Response Handler stopped");
}