#pragma once

#include "Framework/NoCopy.h"

#include <string>
#include <vector>

typedef std::vector<std::string> LogLines;

class ServerLog : public NoCopy
{
public:
	ServerLog();
	virtual ~ServerLog();

	void AddLogLine(const std::string &a_LogLine);
	const LogLines& GetLog() const;

private:
	LogLines m_LogLines;
};
