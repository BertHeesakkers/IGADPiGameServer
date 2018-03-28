#pragma once

#include "ILogger.h"

#include <fstream>
#include <string>
#include <mutex>

class FileLogger : public ILogger
{
public:
	FileLogger();
	FileLogger(const std::string &a_Filename);
	virtual ~FileLogger();
	
	void Open(const std::string &a_Filename, bool a_Truncate = false);
	void SaveLogFile(const std::string &a_Filename, bool a_Purge = false);

private:
	virtual void WriteLine(const std::string &a_Line);

	std::string m_Filename;
	std::ofstream m_LogFile;
	std::mutex m_Mutex;
};
