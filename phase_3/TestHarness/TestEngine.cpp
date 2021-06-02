#include "TestEngine.h"

TestEngine::TestEngine() {
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

}
