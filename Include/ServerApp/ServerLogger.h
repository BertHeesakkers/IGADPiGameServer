#pragma once

#include "Framework/Logging/ILogger.h"

#include <string>
#include <vector>

class ServerLogger : public ILogger
{
public:
	ServerLogger();
	virtual ~ServerLogger();

private:
	void WriteLine(const std::string &a_Line) override;

	std::vector<std::string> m_LogLines;
};