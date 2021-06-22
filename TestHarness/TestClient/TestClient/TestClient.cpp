#include "..\..\TestHarness\Message.h"
#include "..\..\TestHarness\Logger.h"
#include "..\..\TestHarness\Test.h"

void receiver(Socket* s)
{
	Logger::ToConsole("Response listener started");
	while (s->validState())
	{
		// Receive message (without terminator)
		std::string msgStr = s->recvString();
		if (msgStr.length() == 0)
		{
			break;	// Invalid message string
		}
		Message msg = Message::fromString(msgStr);
		msg.clientSocket(s);
		Logger::ToConsole("Received message: " + msg.toString());
		if (msg.type() == MSG_TYPE_SHUTDOWN)
		{
			break;
		}
	}
	Logger::ToConsole("Response listener stopped");

}

Message buildMsg(MsgAddress from, MsgAddress to, std::string body)
{
	Message retVal(from, to);
	retVal.author("Client");
	retVal.type(MSG_TYPE_TEST_REQ);
	retVal.timestamp(Logger::CurrentTimeStamp());
	retVal.body(body);
	return retVal;
}

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	std::thread* receiverThread;

	Logger::ToConsole("Testing comm...");

	struct MsgAddress svrAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT), myAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT + 1);

	ClientSocket client;
	client.connect(svrAddress.IPAddr, svrAddress.Port);
	if (client.validState())
	{
		receiverThread = new std::thread(&receiver, &client);
	}

	Message msgMatt = buildMsg(myAddress, svrAddress, "MattLib.DLL");
	msgMatt.setAttribute(MSG_ATTR_NAME_LOGLEVEL, MSG_LOGLVL_PFD);
	Message msgTanay = buildMsg(myAddress, svrAddress, "TanayDLL.DLL");
	msgTanay.setAttribute(MSG_ATTR_NAME_LOGLEVEL, MSG_LOGLVL_PF);
	Message msgJamie = buildMsg(myAddress, svrAddress, "JamieDLL.DLL");
	msgJamie.setAttribute(MSG_ATTR_NAME_LOGLEVEL, MSG_LOGLVL_PFD);
	client.sendString(msgMatt.toString());
	client.sendString(msgTanay.toString());
	client.sendString(msgJamie.toString());
	getchar();
	client.shutDown();
	client.close();
	system("pause");
}