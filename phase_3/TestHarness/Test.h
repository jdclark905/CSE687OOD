#include <string>
#include "Message.h"

#define MSG_ATTR_NAME_RESULT "test result"		// test result (pass/fail)
#define MSG_ATTR_NAME_DETAIL "test detail"	// test result message (for failure)
#define MSG_ATTR_NAME_LOGLEVEL "log level"		// log level (pass/fail or with message)

#define MSG_TYPE_TEST_REQ "test request"
#define MSG_TYPE_TEST_RESP "test response"

#define MSG_RESULT_PASS "pass"
#define MSG_RESULT_FAIL "fail"

#define MSG_LOGLVL_PF "pass/fail only"
#define MSG_LOGLVL_PFD "pass/fail detail"

#define TEST_PASS true
#define TEST_FAIL false

class Test
{
public:
	Test(const std::string& name) : _name(name){}
	virtual bool test() throw(...) = 0;
	Test* next() const { return _next; }
	void next(Test* next) { _next = next; }
	std::string name() const { return _name; }
	std::string message() const { return _failMessage; }

protected:
	const std::string _name;
	bool _result = TEST_FAIL;
	std::string _failMessage = "";

private:
	Test* _next = nullptr;
};
