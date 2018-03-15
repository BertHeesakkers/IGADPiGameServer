#include "Sleep.h"

void Sleep::MilliSeconds(uint32_t a_MilliSeconds)
{
#ifdef _WIN32
	::Sleep(a_MilliSeconds);
#else
	usleep(a_MilliSeconds * 1000);
#endif
}
