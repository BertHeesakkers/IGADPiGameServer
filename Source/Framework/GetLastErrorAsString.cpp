#include "GetLastErrorAsString.h"

#if defined(PROTO_PLATFORM_WIN32)
#include <windows.h>
#endif

std::string GetLastErrorAsString()
{
	std::string errorMessage;
#if defined(PROTO_PLATFORM_WIN32)
	const DWORD errorMessageID = ::GetLastError();
	if (0 != errorMessageID)
	{
		LPSTR messageBuffer = nullptr;
		const size_t size = ::FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)(&messageBuffer), 0, NULL);
		errorMessage = std::string(messageBuffer, size);
		::LocalFree(messageBuffer);
	}
#elif defined(PROTO_PLATFORM_RASPBERRY_PI)
#endif
	return errorMessage;
}
