#include "FileLogger.h"

#include "../AssertMessage.h"
#include "../DateTimeHelpers.h"
#include "../StringFunctions.h"

#include <boost/format.hpp>

#include <assert.h>
#include <stdarg.h>
#include <thread>

FileLogger::FileLogger()
{
}

FileLogger::FileLogger(const std::string &a_Filename)
{
	m_Filename = a_Filename;
	m_LogFile.open(a_Filename.c_str(), std::ios::out | std::ios::app);
	AssertMessage(m_LogFile.is_open(), StringFormatter::Format("Unable to open [%s] for logging!", a_Filename.c_str()));
}

FileLogger::~FileLogger()
{
}

void FileLogger::Open(const std::string &a_Filename, bool a_Truncate /* = false */)
{
	AssertMessage(!m_LogFile.is_open(), "Log file has already been opened!");
	m_LogFile.open(a_Filename.c_str(), std::ios::out | (a_Truncate ? std::ios::trunc : std::ios::app));
	AssertMessage(m_LogFile.is_open(), StringFormatter::Format("Unable to open [%s] for logging!", a_Filename.c_str()));
	m_Filename = a_Filename;
}

void FileLogger::SaveLogFile(const std::string &a_Filename, bool a_Purge /* = false */)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	std::ofstream logFile(a_Filename, std::ios::out);
	logFile << m_LogFile.rdbuf();
	logFile.close();
	m_LogFile.close();
	Open(m_Filename, a_Purge);
}

void FileLogger::WriteLine(const std::string &a_Line)
{
	std::lock_guard<std::mutex> lock(m_Mutex);

	AssertMessage(m_LogFile.is_open(), "Log file has not been opened!");
	m_LogFile << "[" << GetDateTimeString() << "] " << a_Line << std::endl;
}
