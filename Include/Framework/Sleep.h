#pragma once

#ifdef _WIN32
#  include <Windows.h>
#else
#  include <unistd.h>
#endif
#include <stdint.h>

class Sleep
{
public:
	static void MilliSeconds(uint32_t a_MilliSeconds);
};
