#include "TestEngine.h"

TestEngine::TestEngine() {
}

TestEngine& TestEngine::GetInstance() {
	static TestEngine instance;
	return instance;
}

std::string TestEngine::FormatMessage(assertion item) {
	std::string message = "";
	std::string status = (item.result ? "Passed" : "Failed");
	std::string name = item.function_name + " (" + item.assertion_type + ")";
	if (item.logging_level == PassFailOnly)
		message = status;
	else if (item.logging_level == PassFailMessage)
		message = status + " -> " + name;
	else if (item.logging_level == PassFailMessageWithTimestamp)
		message = GetTimestamp() + ": " + status + " -> " + name;

	return message;
}

std::string TestEngine::GetTimestamp() {
	time_t now;
	struct tm timeinfo;
	char buffer[20];

	time(&now);
	localtime_s(&timeinfo, &now);

	strftime(buffer, 20, "%x %X", &timeinfo);
	return (std::string)buffer;
}

Logger logThis;

void TestEngine::Execute() {

	// unit test class instance
	UnitTest UT = UnitTest();

	// assertions
	std::vector<assertion> list;

	assertion assert1;
	assert1.assertion_type = "equals";
	assert1.function_name = "Test1";
	assert1.logging_level = PassFailMessage;
	assert1.function = &testFunction1;
	
	assertion assert2;
	assert2.assertion_type = "equals";
	assert2.function_name = "Test2";
	assert2.logging_level = PassFailMessage;
	assert2.function = &testFunction2;

	assertion assert3;
	assert3.assertion_type = "equals";
	assert3.function_name = "Test3";
	assert3.logging_level = PassFailMessageWithTimestamp;
	assert3.function = &testFunction3;

	list.push_back(assert1);
	list.push_back(assert2);
	list.push_back(assert3);

	for (int i = 0; i < (int)list.size(); i++) {
		try {

			UT.AssertEquals(list[i].function, 0, list[i]);

			logThis.ToConsole(FormatMessage(list[i]));
			logThis.ToFile(FormatMessage(list[i]));
			
			/*
			* Journal message to file or console.
			* Logger::GetInstance().Journal(entry);
			*/

		}
		catch (const std::exception& e)
		{
			logThis.ToConsole(FormatMessage(list[i]));
			logThis.ToConsole(e.what()); // could add some format for exception message

			logThis.ToFile(FormatMessage(list[i]));
			logThis.ToFile(e.what());
			continue;
		}
	}

	return;
}

TestEngine TestEngine::instance;
