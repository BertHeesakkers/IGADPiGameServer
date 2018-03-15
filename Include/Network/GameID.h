#pragma once

#include <stdint.h>

typedef uint32_t GameID;

inline GameID InvalidGameID()
{
	return static_cast<GameID>(-1);
}
