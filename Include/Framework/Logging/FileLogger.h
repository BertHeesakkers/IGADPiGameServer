#pragma once

#include "ILogger.h"

#include <fstream>
#include <string>

class FileLogger : public ILogger
{
public:
	FileLogger();
	FileLogger(const std::string &a_Filename);
	virtual ~FileLogger();
	
	void Open(const std::string &a_Filename);

private:
	virtual void WriteLine(const std::string &a_Line);

	std::ofstream m_LogFile;
};
