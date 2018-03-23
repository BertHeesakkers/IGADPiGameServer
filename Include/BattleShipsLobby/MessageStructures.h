#pragma once

#include <stdint.h>

struct ShipPlacement
{
	const static short UP = 0;
	const static short LEFT = 1;
	const static short DOWN = 2;
	const static short RIGHT = 3;

	short m_Ship;
	uint32_t m_X;
	uint32_t m_Y;
	short m_Direction;
};