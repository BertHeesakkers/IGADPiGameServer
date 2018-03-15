#include "ConsoleLogger.h"

#include "../DateTimeHelpers.h"

#include <iostream>

ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::WriteLine(const std::string &a_Line)
{
	std::cout << "[" << GetDateTimeString() << "] " << a_Line << std::endl;
}
