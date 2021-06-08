
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "Message.h"
#include "BlockingQueue.h"

#define NUM_TEST_THREADS 3

using std::cout;

std::thread msg_thread;
std::mutex cout_mutex;
BlockingQueue<Message> msg_queue;
bool stopTestRunner[NUM_TEST_THREADS];
bool stopMsgHandler = false;

// Print to console
void print(const std::string& msg)
{
	std::lock_guard<std::mutex> lock(cout_mutex);
	cout << msg << '\n';
}

/*************************/
/* Functions for threads */
/*************************/

// Message handler
void msgHandler(){
	Message msg;
	while (!stopMsgHandler)
	{
		msg = msg_queue.dequeue();
		print(msg.describe());
	}
}

// Test runner
void testRunner(int nr){
	Message msg;
	msg._source = "testRunner[" + std::to_string(nr) + "]";
	msg._dest = "main";
	msg._type = MSG_TYPE_STATUS;
	msg.setDateTimeNow();
	msg._body = "Ready";
	msg_queue.enqueue(msg);
	while (!stopTestRunner[nr]){}
	msg.setDateTimeNow();
	msg._body = "Stopped";
	msg_queue.enqueue(msg);
	stopTestRunner[nr] = false;
}

/********************/
/* Main entry point */
/********************/
int main(int argc, char *argv[])
{
	// Startup message handler
	std::thread(msgHandler).detach();

	// Startup test runner thread pool
	for (int i = 0; i < NUM_TEST_THREADS; i++)
	{
		print("main creating testRunner[" + std::to_string(i) + "]");
		stopTestRunner[i] = false;
		std::thread t(testRunner, i);
		t.detach();
	}

	std::this_thread::sleep_for(std::chrono::seconds(5));
	for (int i = 0; i < NUM_TEST_THREADS; i++)
	{
		stopTestRunner[i] = true;
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));

	stopMsgHandler = true;

	system("pause");
}
