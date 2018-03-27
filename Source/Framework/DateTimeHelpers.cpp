#include "DateTimeHelpers.h"

#include <locale>

#if defined(WIN32)
#pragma warning(push)  
#pragma warning(disable : 4996)
#endif

std::string GetDateTimeString()
{
	std::time_t rawTime;
	std::time(&rawTime);

	char buffer[80];
	std::strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", std::localtime(&rawTime));
	return std::string(buffer);
}

std::string GetDateTimeFileString()
{
	std::time_t rawTime;
	std::time(&rawTime);

	char buffer[80];
	std::strftime(buffer, 80, "%Y%m%d_%H%M%S", std::localtime(&rawTime));
	return std::string(buffer);
}

#if defined(WIN32)
#pragma warning(pop)
#endif
