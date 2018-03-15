#include "FileLogger.h"

#include "../AssertMessage.h"
#include "../DateTimeHelpers.h"
#include "../StringFunctions.h"

#include <boost/format.hpp>

#include <assert.h>
#include <stdarg.h>

FileLogger::FileLogger()
{
}

FileLogger::FileLogger(const std::string &a_Filename)
{
	m_LogFile.open(a_Filename.c_str(), std::ios::out | std::ios::app);
	AssertMessage(m_LogFile.is_open(), StringFormatter::Format("Unable to open [%s] for logging!", a_Filename.c_str()));
}

FileLogger::~FileLogger()
{
}

void FileLogger::Open(const std::string &a_Filename)
{
	AssertMessage(!m_LogFile.is_open(), "Log file has already been opened!");
	m_LogFile.open(a_Filename.c_str(), std::ios::out | std::ios::app);
	AssertMessage(m_LogFile.is_open(), StringFormatter::Format("Unable to open [%s] for logging!", a_Filename.c_str()));
}

void FileLogger::WriteLine(const std::string &a_Line)
{
	AssertMessage(m_LogFile.is_open(), "Log file has not been opened!");
	m_LogFile << "[" << GetDateTimeString() << "] " << a_Line << std::endl;
}
