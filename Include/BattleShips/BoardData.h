#pragma once

#include "EShip.h"
#include "EDirection.h"

#include <stdint.h>
#include <vector>

struct BoardDataItem
{
	EShip m_Ship;
	uint32_t m_X;
	uint32_t m_Y;
	EDirection m_Direction;
	std::vector<bool> m_Damage;
	short m_Extra;
};

struct BoardData
{
	std::vector<BoardDataItem> m_Items;
};
