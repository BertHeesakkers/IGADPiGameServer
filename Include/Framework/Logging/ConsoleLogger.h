#pragma once

#include "ILogger.h"

#include <string>

class ConsoleLogger : public ILogger
{
public:
	ConsoleLogger();
	virtual ~ConsoleLogger();

	
private:
	virtual void WriteLine(const std::string &a_Line);
};
