#pragma once

#include "Comm.h"
#include "TestHandler.h"
#include "Message.h"

// Functor for handling client connections and messages
class ClientHandler
{
public:
	ClientHandler(TestHandler& testHandler);
	void operator()(Socket& s);

private:
	TestHandler& _testHandler;
};