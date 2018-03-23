#include "ShipPlacementInfo.h"

#include "GetShipSize.h"

ShipPlacementInfo::ShipPlacementInfo(const ShipPlacementInfo &a_Info)
	: m_Index(a_Info.m_Index)
	, m_Player(a_Info.m_Player)
	, m_Ship(a_Info.m_Ship)
	, m_X(a_Info.m_X)
	, m_Y(a_Info.m_Y)
	, m_Direction(a_Info.m_Direction)
	, m_ShieldActive(a_Info.m_ShieldActive)
{
	const uint32_t shipSize = GetShipSize(m_Ship);
	m_Damage = new bool[shipSize];
	for (uint32_t i = 0; i < shipSize; ++i)
	{
		m_Damage[i] = a_Info.m_Damage[i];
	}
}

ShipPlacementInfo::ShipPlacementInfo(uint32_t a_Index, EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction)
	: m_Index(a_Index)
	, m_Player(a_Player)
	, m_Ship(a_Ship)
	, m_X(a_X)
	, m_Y(a_Y)
	, m_Direction(a_Direction)
	, m_ShieldActive(false)
{
	const uint32_t shipSize = GetShipSize(m_Ship);
	m_Damage = new bool[shipSize];
	for (uint32_t i = 0; i < shipSize; ++i)
	{
		m_Damage[i] = false;
	}
}

ShipPlacementInfo::~ShipPlacementInfo()
{
	delete[] m_Damage;
}
