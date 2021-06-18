#include "ClientHandler.h"

ClientHandler::ClientHandler(TestHandler& testHandler) : _testHandler(testHandler)
{

}

void ClientHandler::operator()(Socket& s)
{
	Logger::ToConsole("ClientHandler started");
	while (s.validState())
	{
		std::string msgStr = s.recvString(MSG_CHAR_TERM);
		if (msgStr.length() == 0)
		{
			break;	// Invalid message string
		}
		Message msg = Message::fromString(msgStr);
		msg.clientSocket(&s);
		Logger::ToConsole("ClientHandler received message: " + msg.toString());
		_testHandler.enqueue(msg);
		if (msg.type() == MSG_TYPE_SHUTDOWN)
		{
			break;
		}
	}
	Logger::ToConsole("ClientHandler terminating");
}
