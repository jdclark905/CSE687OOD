
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Message.h"

#define NUM_TEST_THREADS 3
#define SERVER_PORT "12345"

using std::cout;

std::thread msg_thread;
std::mutex cout_mutex, msg_mutex, test_mutex;
std::condition_variable msg_condv, test_condv;
std::queue<std::string> msg_queue, test_queue;
bool stop = false;

// Socket listener
void listener(){

}

// Message handler
void msgHandler(){
	
}

// Test runner
void testRunner(int nr){
	cout << "testRunner(" << nr << ") started.\n";
	while (!stop){}
	cout << "testRunner(" << nr << ") stopping.\n";
}

// Main entry point
int main(int argc, char *argv[])
{
	/*
	WSADATA wsaData;
	int iResult;
	SOCKET listenSocket = NULL;

	// Initialize socket listener
	cout << "Initializing socket listener...\n";
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup failed: " << WSAGetLastError() << '\n';
		return 1;
	}
	listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET)
	{
		cout << "Listen socket creation failed: " << WSAGetLastError() << '\n';
		return 1;
	}
	

	// Startup message handler
	msg_thread = std::thread(msgHandler);
	*/

	// Startup test runner thread pool
	for (int i = 0; i < NUM_TEST_THREADS; i++)
	{
		std::thread t(testRunner, i);
		t.detach();
	}

	std::this_thread::sleep_for(std::chrono::seconds(5));
	stop = true;

	system("pause");
}
