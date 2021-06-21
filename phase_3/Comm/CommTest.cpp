#include "..\TestHarness\Comm.h"
#include "..\TestHarness\Message.h"

int main()
{
	WSAData wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	Logger::ToConsole("Testing comm...");

	struct MsgAddress svrAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT), myAddress(DEFAULT_LISTEN_IP, DEFAULT_LISTEN_PORT+1);
	
	ClientSocket client;
	client.connect(svrAddress.IPAddr, svrAddress.Port);

	Message msg(myAddress, svrAddress);
	msg.type(MSG_TYPE_TEST_REQ);
	msg.author("Client");
	msg.timestamp(Logger::CurrentTimeStamp());
	msg.body("MattLib.DLL");
	client.sendString(msg.toString());

	std::this_thread::sleep_for(std::chrono::seconds(2));
	client.shutDown();
	client.close();
	system("pause");
}