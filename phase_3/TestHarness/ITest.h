#include <string>
#include "Message.h"

class ITest
{
public:
	enum TestResult
	{
		fail,
		pass,
		undefined
	};

	enum LogLevel {
		PassFailOnly,
		PassFailMessage,
	};

	struct TestResultInfo
	{
		TestResult result = undefined;
		LogLevel logLevel = PassFailOnly;
		std::string message = "";
	};

	ITest(const std::string& name) : _name(name){}
	virtual bool test() = 0;
	ITest* next() const { return _next; }
	void next(ITest* next) { _next = next; }
	std::string name() const { return _name; }
	void causedException(const std::exception& ex) { _message += " exception"; }
	void appendMessage(Message& msg)
	{
		// Append test result
		msg.setAttribute("test result", _pass ? "pass" : "fail");

		// Append test result message
		if (_logLevel == PassFailMessage)
		{
			msg.setAttribute("test result message", _message);
			if (_exceptionMessage != "")
			{
				msg.setAttribute("test result exception", _exceptionMessage);
			}
		}
	}

protected:
	std::string _name = "";
	LogLevel _logLevel = PassFailOnly;
	bool _pass = false;
	std::string _message = "";
	std::string _exceptionMessage = "";

private:
	ITest* _next = nullptr;
};
