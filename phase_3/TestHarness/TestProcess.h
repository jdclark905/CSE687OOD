#include <windows.h>
#include <string>
#include <iostream>
#include <functional>

class TestProcess
{
public:
	TestProcess();
	virtual ~TestProcess() {}
	void setTitle(const std::string& title);
	void setCmdLine(const std::string& cmdLine);
	bool create(const std::string& path = "");
	HANDLE getHandle() const;
private:
	STARTUPINFO startInfo_;
	PROCESS_INFORMATION procInfo_;
	std::string title_;
	std::string commandLine_;
};

inline TestProcess::TestProcess()
{
	GetStartupInfo(&startInfo_);
	startInfo_.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE;
}

inline void TestProcess::setTitle(const std::string& title)
{
	title_ = title;
	startInfo_.lpTitle = const_cast<LPSTR>(title_.c_str());
}

inline void TestProcess::setCmdLine(const std::string& cmdLine)
{
	commandLine_ = cmdLine;
}

inline bool TestProcess::create(const std::string& appName)
{
	LPCTSTR applic = NULL;
	LPTSTR cmdLine = const_cast<LPTSTR>(commandLine_.c_str());
	LPSECURITY_ATTRIBUTES prosec = NULL;
	LPSECURITY_ATTRIBUTES thrdsec = NULL;
	BOOL inheritHandles = false;
	DWORD createFlags = CREATE_NEW_CONSOLE;
	LPVOID environment = NULL;
	LPCTSTR currentPath = NULL;
	LPSTARTUPINFO pStartInfo = &startInfo_;
	LPPROCESS_INFORMATION pPrInfo = &procInfo_;

	BOOL OK =
		CreateProcess(applic, cmdLine, prosec, thrdsec, inheritHandles,
		createFlags, environment, currentPath, pStartInfo, pPrInfo);
	return OK;
}

inline HANDLE TestProcess::getHandle() const
{
	return procInfo_.hProcess;
}