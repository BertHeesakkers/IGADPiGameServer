#include "ServerLog.h"

#include "Framework/DateTimeHelpers.h"

#include <sstream>

namespace
{
	std::string GetTimeStamp()
	{
		std::stringstream ss;
		ss << "[" << GetDateTimeString() << "]";
		return ss.str();
	}
}

ServerLog::ServerLog()
{
}

ServerLog::~ServerLog()
{
}

void ServerLog::AddLogLine(const std::string &a_LogLine)
{
	std::stringstream ss;
	ss << GetTimeStamp() << " " << a_LogLine << "\n";
	m_LogLines.push_back(a_LogLine);
}

const LogLines& ServerLog::GetLog() const
{
	return m_LogLines;
}