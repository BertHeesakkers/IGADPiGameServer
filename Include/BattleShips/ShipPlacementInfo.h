#pragma once

#include "Network/EPlayer.h"
#include "EDirection.h"
#include "EShip.h"

#include <stdint.h>
#include <vector>

struct ShipPlacementInfo
{
	ShipPlacementInfo(const ShipPlacementInfo &a_Info);
	ShipPlacementInfo(uint32_t a_Index, EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction);
	~ShipPlacementInfo();

	const uint32_t m_Index;
	const EPlayer m_Player;
	const EShip m_Ship;
	uint32_t m_X;
	uint32_t m_Y;
	EDirection m_Direction;
	bool *m_Damage;
	bool m_ShieldActive;
};

typedef std::vector<ShipPlacementInfo*> ShipPlacementVector;
