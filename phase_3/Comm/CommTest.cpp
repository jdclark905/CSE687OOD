#include "..\TestHarness\Comm.h"
#include "..\TestHarness\Message.h"
#include "..\TestHarness\Test.h"

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

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	std::thread* receiverThread;
	
	Logger::ToConsole("Testing comm...");

	struct MsgAddress svrAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT), myAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT+1);
	
	ClientSocket client;
	client.connect(svrAddress.IPAddr, svrAddress.Port);
	if (client.validState())
	{
		receiverThread = new std::thread(&receiver, &client);
	}

	Message msg(myAddress, svrAddress);
	msg.type(MSG_TYPE_TEST_REQ);
	msg.author("Client");
	msg.timestamp(Logger::CurrentTimeStamp());
	msg.body("MattLib.DLL");
	client.sendString(msg.toString());
	getchar();
	client.shutDown();
	client.close();
	system("pause");
}