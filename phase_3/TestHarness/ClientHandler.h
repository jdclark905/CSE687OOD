#pragma once

#include "Comm.h"
#include "Message.h"

// Functor for handling client connections and messages
class ClientHandler
{
public:
	ClientHandler(BlockingQueue<Message>& requestQueue, BlockingQueue<Message>& responseQueue);
	void start();
	void shutdown();
	void enqueue(Message msg);

	// Functor for receiving client messages
	class MsgReceiver
	{
	public:
		MsgReceiver(BlockingQueue<Message>&);
		void operator()(Socket& s);
	private:
		BlockingQueue<Message>& _requestQueue;
	};

private:
	bool _running;
	BlockingQueue<Message>& _requestQueue;
	BlockingQueue<Message>& _responseQueue;
	ServerSocket _listener;

	// Function for receiving client messages (one thread per connection)
	void receiver(Socket& s);
	// Function to handle response messages
	void responder();
};