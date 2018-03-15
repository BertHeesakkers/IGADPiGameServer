#include "ServerLogger.h"

ServerLogger::ServerLogger()
{
}

ServerLogger::~ServerLogger()
{
}

void ServerLogger::WriteLine(const std::string &a_Line)
{
	m_LogLines.push_back(a_Line);
}
