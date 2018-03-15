#pragma once

#include <stdint.h>

typedef uint32_t ClientID;

inline ClientID InvalidClientID()
{
	return static_cast<ClientID>(-1);
}
