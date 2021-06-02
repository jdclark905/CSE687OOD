
#include "TestEngine.h"
#include <iostream>
#include <queue>
#include "TestProcess.h"
#include <winsock.h>

#define CMD_ARG_APPNAME "TestHarness"
#define CMD_ARG_CLIENT "client"

using std::cout;

void runHost();
void runClient();

DWORD myProcId;

// Client parameters to include host IP
// address and port for communication channel
int main(int argc, char *argv[])
{
	// Set current process ID
	myProcId = GetCurrentProcessId();

	// No arguments (other than process name) runs as test harness host
	if (argc == 1){
		cout << "Host process (PID " << myProcId << ") starting\n";
		runHost();
	}
	else {
		runClient();
	}
	
}

// Run-time instance is a test harness host process
void runHost()
{
	const int numClients = 4;
	TestProcess testClient[numClients];
	STARTUPINFO si;
	GetStartupInfo(&si);
	std::queue<HANDLE> rdyQ;

	// Create child test processes
	for (int t = 0; t < numClients; t++)
	{
		testClient[t].setTitle("Test Client " + std::to_string(t));
		testClient[t].setCmdLine("TestHarness.exe " + std::to_string(t));
		cout << "Creating client process " << t << '\n';
		if (testClient[t].create())
		{
			cout << "Client " << t << " created\n";
		}
	}

	for (int i = 0; i < numClients; i++)
	{
		WaitForSingleObject(testClient[i].getHandle(), 10000);
		cout << "Client " << i << " exited\n";
	}

	system("pause");
}

// Run-time instance is a test harness client process
void runClient()
{
	cout << "Client process ID " << myProcId << " started\n";
	Sleep(5000);
}