#include "BattleShipsBoard.h"

#include "BoardData.h"
#include "GetShipSize.h"

#include <algorithm>
#include <assert.h>

namespace
{
	void ClearTile(BattleShipsBoard::Tile &a_Tile)
	{
		a_Tile.m_Value = ' ';
	}

	bool IsEmpty(const BattleShipsBoard::Tile &a_Tile)
	{
		return a_Tile.m_Value == ' ';
	}

	bool IsMine(const BattleShipsBoard::Tile &a_Tile)
	{
		return a_Tile.m_Value == 'm';
	}

	bool CheckCoordinates(uint32_t a_X, uint32_t a_Y, uint32_t a_Width, uint32_t a_Height)
	{
		return a_X < a_Width && a_Y < a_Height;
	}

	bool CheckIndex(int32_t a_Index, uint32_t a_Width, uint32_t a_Height)
	{
		return a_Index >= 0 && a_Index <= static_cast<int32_t>(a_Width * a_Height);
	}

	void NextCoordinate(uint32_t &a_X, uint32_t &a_Y, EDirection a_Direction, uint32_t a_Width, uint32_t a_Height)
	{
		(void)a_Width;
		(void)a_Height;

		switch (a_Direction)
		{
			case EDirection_Left:
				{
					--a_X;
				}
				break;
			case EDirection_Right:
				{
					++a_X;
				}
			break;
			case EDirection_Up:
				{
					--a_Y;
				}
			break;
				case EDirection_Down:
				{
					++a_Y;
				}
			break;
			case EDirection_Count:
			default:
				{
					assert(false);
				}
			break;
		}
	}

	uint32_t NextIndex(const uint32_t a_Index, EDirection a_Direction, uint32_t a_Width)
	{
		uint32_t nextIndex = a_Index;
		switch (a_Direction)
		{
			case EDirection_Left:
				{
					nextIndex--;
				}
				break;
			case EDirection_Right:
				{
					nextIndex++;
				}
				break;
			case EDirection_Up:
				{
					nextIndex -= a_Width;
				}
				break;
			case EDirection_Down:
				{
					nextIndex += a_Width;
				}
				break;
			case EDirection_Count:
			default:
				{
					assert(false);
				}
				break;
		}
		return nextIndex;
	}

	bool ShipOnPlayfield(EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction, uint32_t a_Width, uint32_t a_Height)
	{
		bool success = CheckCoordinates(a_X, a_Y, a_Width, a_Height);
		if (success)
		{
			const int32_t shipSize = static_cast<int32_t>(GetShipSize(a_Ship));
			uint32_t startIndex = a_X + a_Y * a_Width;
			for (int32_t i = 0; i < shipSize && success; ++i)
			{
				success = CheckIndex(startIndex, a_Width, a_Height);
				if (success)
				{
					startIndex = NextIndex(startIndex, a_Direction, a_Width);
				}
			}
		}
		return success;
	}

	bool ShipDestroyed(const ShipPlacementInfo &a_Info)
	{
		uint32_t damageAmount = 0;
		const uint32_t shipSize = GetShipSize(a_Info.m_Ship);
		for (uint32_t i = 0; i < shipSize; ++i)
		{
			damageAmount += a_Info.m_Damage[i] ? 1 : 0;
		}
		return damageAmount == shipSize;
	}

	class ShipPredicate
	{
	public:
		ShipPredicate(EPlayer a_Player, EShip a_Ship)
			: m_Player(a_Player)
			, m_Ship(a_Ship)
		{
		}

		bool operator()(ShipPlacementInfo *a_Info) const
		{
			return  a_Info->m_Player == m_Player && a_Info->m_Ship == m_Ship;
		}

	private:
		const EPlayer m_Player;
		const EShip m_Ship;
	};

	class ShipIndexPredicate
	{
	public:
		ShipIndexPredicate(uint32_t a_Index)
			: m_Index(a_Index)
		{
		}

		bool operator()(ShipPlacementInfo *a_Info) const
		{
			return  a_Info->m_Index == m_Index;
		}

	private:
		const uint32_t m_Index;
	};
}

uint32_t BattleShipsBoard::ms_AddedShips = 0;

BattleShipsBoard::BattleShipsBoard(uint32_t a_Width, uint32_t a_Height)
	: m_Width(a_Width)
	, m_Height(a_Height)
{
	m_Tiles = new Tile[m_Width * m_Height];
	Clear();
}

BattleShipsBoard::~BattleShipsBoard()
{
	for (ShipPlacementVector::iterator pos = m_Ships.begin(); pos != m_Ships.end(); ++pos)
	{
		ShipPlacementInfo *info = *pos;
		delete info;
	}
	delete [] m_Tiles;
}

void BattleShipsBoard::Clear()
{
	for (uint32_t y = 0; y < m_Height; ++y)
	{
		for (uint32_t x = 0; x < m_Width; ++x)
		{
			const uint32_t index = y * m_Width + x;
			ClearTile(m_Tiles[index]);
		}
	}
}

ECollision BattleShipsBoard::AddShip(EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction)
{
	//const bool hasShip = HasShip(a_Player, a_Ship);
	const ECollision collision = CanPlaceShip(a_Player, a_Ship, a_X, a_Y, a_Direction);
	if (!HasShip(a_Player, a_Ship) && ECollision_None == collision)
	{
		ShipPlacementInfo *info = new ShipPlacementInfo(ms_AddedShips++, a_Player, a_Ship, a_X, a_Y, a_Direction);
		m_Ships.push_back(info);
		//const uint32_t shipIndex = static_cast<uint32_t>(m_Ships.size());

		const uint32_t shipSize = GetShipSize(a_Ship);
		uint32_t startIndex = a_X + a_Y * m_Width;
		for (uint32_t i = 0; i < shipSize; ++i)
		{
			m_Tiles[startIndex].m_Value = static_cast<char>(info->m_Index + 65);
			startIndex = NextIndex(startIndex, a_Direction, m_Width);
		}
	}
	return collision;
}

bool BattleShipsBoard::RemoveShip(EPlayer a_Player, EShip a_Ship)
{
	bool success = false;
	ShipPlacementVector::iterator pos = std::find_if(m_Ships.begin(), m_Ships.end(), ShipPredicate(a_Player, a_Ship));
	if (m_Ships.end() != pos)
	{
		const ShipPlacementInfo &info = **pos;
		const uint32_t shipSize = GetShipSize(info.m_Ship);
		uint32_t startIndex = info.m_X + info.m_Y * m_Width;
		for (uint32_t i = 0; i < shipSize; ++i)
		{
			ClearTile(m_Tiles[startIndex]);
			startIndex = NextIndex(startIndex, info.m_Direction, m_Width);
		}

		m_Ships.erase(pos);
		success = true;
	}
	return success;
}

ECollision BattleShipsBoard::MoveShip(EPlayer a_Player, EShip a_Ship, bool a_Forward)
{
	ECollision collision = ECollision_None;

	ShipPlacementInfo *info = FindShip(a_Player, a_Ship);
	bool success = 0 != info;
	if (success)
	{
		int32_t x = info->m_X;
		int32_t y = info->m_Y;
		if (EDirection_Left == info->m_Direction)
		{
			x += a_Forward ? 1 : -1;
		}
		else if (EDirection_Right == info->m_Direction)
		{
			x += a_Forward ? -1 : 1;
		}
		else if (EDirection_Up == info->m_Direction)
		{
			y += a_Forward ? 1 : -1;
		}
		else if (EDirection_Down == info->m_Direction)
		{
			y += a_Forward ? -1 : 1;
		}
		if (ShipOnPlayfield(info->m_Ship, x, y, info->m_Direction, m_Width, m_Height))
		{
			ShipPlacementInfo newShipPlacement(*info);
			newShipPlacement.m_X = x;
			newShipPlacement.m_Y = y;
			RemoveShip(a_Player, a_Ship);
			collision = AddShip(a_Player, newShipPlacement.m_Ship, newShipPlacement.m_X, newShipPlacement.m_Y, newShipPlacement.m_Direction);
			if (ECollision_None == collision)
			{
				info = FindShip(a_Player, newShipPlacement.m_Ship);

				const uint32_t shipSize = GetShipSize(info->m_Ship);
				for (uint32_t i = 0; i < shipSize; ++i)
				{
					info->m_Damage[i] = newShipPlacement.m_Damage[i];
				}
				info->m_ShieldActive = newShipPlacement.m_ShieldActive;
			}
			else if (ECollision_Ship == collision)
			{
				// find colliding ship
				ShipPlacementInfo* collidingShip = FindCollidingShip(newShipPlacement);
				const uint32_t collidingShipSize = GetShipSize(collidingShip->m_Ship);
				const uint32_t shipSize = GetShipSize(newShipPlacement.m_Ship);
				if (collidingShipSize > shipSize)
				{
					// do nothing; new ship wasn't added again yet
				}
				else if (collidingShipSize == shipSize)
				{
					// same size; destroy both ships
					for (uint32_t i = 0; i < shipSize; ++i)
					{
						collidingShip->m_Damage[i] = true;
						newShipPlacement.m_Damage[i] = true;
					}
				}
				else
				{
					// destroy the other ship
					for (uint32_t i = 0; i < shipSize; ++i)
					{
						collidingShip->m_Damage[i] = true;
					}
					collision = AddShip(a_Player, newShipPlacement.m_Ship, newShipPlacement.m_X, newShipPlacement.m_Y, newShipPlacement.m_Direction);
				}
			}
			else if (ECollision_Unknown == collision)
			{
				// do nothing; new ship wasn't added again yet
				ClearMine(newShipPlacement);
			}
		}
		else
		{
			const uint32_t shipSize = GetShipSize(info->m_Ship);
			for (uint32_t i = 0; i < shipSize; ++i)
			{
				info->m_Damage[i] = true;
			}
			collision = ECollision_World;
		}
	}
	return collision;
}

ECollision BattleShipsBoard::RotateShip(EPlayer a_Player, EShip a_Ship, bool a_Starboard)
{
	ECollision collision = ECollision_None;

	ShipPlacementInfo *info = FindShip(a_Player, a_Ship);
	if (0 != info)
	{
		EDirection direction = EDirection_Invalid;
		if (EDirection_Left == info->m_Direction)
		{
			direction = a_Starboard ? EDirection_Up : EDirection_Down;
		}
		else if (EDirection_Right == info->m_Direction)
		{
			direction = a_Starboard ? EDirection_Down : EDirection_Up;
		}
		else if (EDirection_Up == info->m_Direction)
		{
			direction = a_Starboard ? EDirection_Right : EDirection_Left;
		}
		else if (EDirection_Down == info->m_Direction)
		{
			direction = a_Starboard ? EDirection_Left : EDirection_Right;
		}

		if (ShipOnPlayfield(info->m_Ship, info->m_X, info->m_Y, direction, m_Width, m_Height))
		{
			ShipPlacementInfo newShipPlacement(*info);
			newShipPlacement.m_Direction = direction;
			RemoveShip(a_Player, a_Ship);
			collision = AddShip(a_Player, newShipPlacement.m_Ship, newShipPlacement.m_X, newShipPlacement.m_Y, newShipPlacement.m_Direction);
			if (ECollision_None == collision)
			{
				info = FindShip(a_Player, newShipPlacement.m_Ship);

				const uint32_t shipSize = GetShipSize(info->m_Ship);
				for (uint32_t i = 0; i < shipSize; ++i)
				{
					info->m_Damage[i] = newShipPlacement.m_Damage[i];
				}
				info->m_ShieldActive = newShipPlacement.m_ShieldActive;
			}
			else if (ECollision_Ship == collision)
			{
				// find colliding ship
				ShipPlacementInfo* collidingShip = FindCollidingShip(newShipPlacement);
				const uint32_t collidingShipSize = GetShipSize(collidingShip->m_Ship);
				const uint32_t shipSize = GetShipSize(newShipPlacement.m_Ship);
				if (collidingShipSize > shipSize)
				{
					// do nothing; new ship wasn't added again yet
				}
				else if (collidingShipSize == shipSize)
				{
					// same size; destroy both ships
					for (uint32_t i = 0; i < shipSize; ++i)
					{
						collidingShip->m_Damage[i] = true;
						newShipPlacement.m_Damage[i] = true;
					}
				}
				else
				{
					// destroy the other ship
					for (uint32_t i = 0; i < shipSize; ++i)
					{
						collidingShip->m_Damage[i] = true;
					}
					collision = AddShip(a_Player, newShipPlacement.m_Ship, newShipPlacement.m_X, newShipPlacement.m_Y, newShipPlacement.m_Direction);
				}
			}
			else if (ECollision_World == collision)
			{
				const uint32_t shipSize = GetShipSize(info->m_Ship);
				for (uint32_t i = 0; i < shipSize; ++i)
				{
					info->m_Damage[i] = true;
				}
			}
		}
		else
		{
			const uint32_t shipSize = GetShipSize(info->m_Ship);
			for (uint32_t i = 0; i < shipSize; ++i)
			{
				info->m_Damage[i] = true;
			}
			collision = ECollision_World;
		}
	}
	return collision;
}

bool BattleShipsBoard::IsValidPosition(uint32_t a_X, uint32_t a_Y)
{
	return (a_X < m_Width && a_Y < m_Height);
}

ECollision BattleShipsBoard::Shoot(uint32_t a_X, uint32_t a_Y)
{
	ECollision collision = ECollision_None;
	if (CheckCoordinates(a_X, a_Y, m_Width, m_Height))
	{
		const uint32_t index = a_X + a_Y * m_Width;
		if (!IsEmpty(m_Tiles[index]))
		{
			ShipPlacementVector::iterator pos = std::find_if(m_Ships.begin(), m_Ships.end(), ShipIndexPredicate(m_Tiles[index].m_Value - 65));
			if (m_Ships.end() != pos)
			{
				ShipPlacementInfo &info = **pos;

				// determine part of ship that was hit
				const uint32_t hitIndex = a_X + a_Y * m_Width;
				uint32_t startIndex = info.m_X + info.m_Y * m_Width;
				uint32_t shipPartIndex = 0;
				while (startIndex != hitIndex)
				{
					startIndex = NextIndex(startIndex, info.m_Direction, m_Width);
					shipPartIndex++;
				}
				info.m_Damage[shipPartIndex] = true;

				collision = ECollision_Ship;
			}
			else if (IsMine(m_Tiles[index]))
			{
				collision = ECollision_Unknown;
			}
		}
	}
	return collision;
}

void BattleShipsBoard::PlaceMine(uint32_t a_X, uint32_t a_Y)
{
	const uint32_t index = a_Y * m_Width + a_X;
	m_Tiles[index].m_Value = 'm';
}

void BattleShipsBoard::RepairShip(EPlayer a_Player, EShip a_Ship)
{
	ShipPlacementInfo *info = FindShip(a_Player, a_Ship);
	bool success = 0 != info;
	if (success)
	{
		const uint32_t shipSize = GetShipSize(a_Ship);
		for (uint32_t i = 0; i < shipSize; ++i)
		{
			info->m_Damage[i] = false;
		}
	}
}

void BattleShipsBoard::ShieldShip(EPlayer a_Player, EShip a_Ship)
{
	ShipPlacementInfo *info = FindShip(a_Player, a_Ship);
	bool success = 0 != info;
	if (success)
	{
		const uint32_t shipSize = GetShipSize(a_Ship);
		for (uint32_t i = 0; i < shipSize; ++i)
		{
			info->m_ShieldActive = true;
		}
	}
}

void BattleShipsBoard::Radar(uint32_t a_X, uint32_t a_Y, char *a_RadarInfo)
{
	for (int32_t x = -1; x < 2; ++x)
	{
		for (int32_t y = -1; y < 2; ++y)
		{
			const uint32_t radarIndex = (x + 1) + (y + 1) * 3;
			a_RadarInfo[radarIndex] = 0;
			const int32_t index = (a_X + x) + (a_Y + y) * m_Width;
			if (CheckIndex(index, m_Width, m_Height))
			{
				a_RadarInfo[radarIndex] = m_Tiles[index].m_Value;
			}
		}
	}
}

bool BattleShipsBoard::UpdateShips()
{
	bool shipDestroyed = false;
	for (ShipPlacementVector::iterator pos = m_Ships.begin(); pos != m_Ships.end(); )
	{
		ShipPlacementInfo &info = **pos;
		shipDestroyed = ShipDestroyed(info);
		if (shipDestroyed)
		{
			RemoveShip(info.m_Player, info.m_Ship);
			pos = m_Ships.begin();
		}
		else
		{
			++pos;
		}
	}
	return shipDestroyed;
}

void BattleShipsBoard::GetData(EPlayer a_Player, BoardData &a_BoardData)
{
	for (ShipPlacementVector::iterator pos = m_Ships.begin(); pos != m_Ships.end(); ++pos)
	{
		ShipPlacementInfo &info = **pos;
		if (a_Player == info.m_Player)
		{
			BoardDataItem item;
			item.m_Ship = info.m_Ship;
			item.m_X = info.m_X;
			item.m_Y = info.m_Y;
			item.m_Direction = info.m_Direction;
			for (uint32_t i = 0; i < GetShipSize(info.m_Ship); ++i)
			{
				item.m_Damage.push_back(info.m_Damage[i]);
			}
			item.m_Extra = info.m_ShieldActive;

			a_BoardData.m_Items.push_back(item);
		}
	}
}

ECollision BattleShipsBoard::CanPlaceShip(EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction)
{
	(void)a_Player;

	ECollision collision = ECollision_None;

	const int32_t shipSize = static_cast<int32_t>(GetShipSize(a_Ship));
	for (int32_t i = 0; i < shipSize; ++i)
	{
		collision = CheckCoordinates(a_X, a_Y, m_Width, m_Height) ? ECollision_None : ECollision_World;
		int32_t tileIndex = a_X + a_Y * m_Width;
		collision = IsMine(m_Tiles[tileIndex]) ? ECollision_Unknown : (IsEmpty(m_Tiles[tileIndex]) ? collision : ECollision_Ship);
		if (ECollision_None == collision)
		{
			NextCoordinate(a_X, a_Y, a_Direction, m_Width, m_Height);
		}
	}
	return collision;
}

bool BattleShipsBoard::HasShip(EPlayer a_Player, EShip a_Ship) const
{
	return (m_Ships.end() != std::find_if(m_Ships.begin(), m_Ships.end(), ShipPredicate(a_Player, a_Ship)));
}

bool BattleShipsBoard::HasShipsLeft(EPlayer a_Player) const
{
	bool hasShipsLeft = false;
	for (uint32_t shipIndex = 0; shipIndex < static_cast<uint32_t>(EShip_Count); ++shipIndex)
	{
		const EShip ship = static_cast<EShip>(shipIndex);
		hasShipsLeft = hasShipsLeft || HasShip(a_Player, ship);
	}
	return hasShipsLeft;
}

ShipPlacementInfo& BattleShipsBoard::GetShip(EPlayer a_Player, EShip a_Ship)
{
	ShipPlacementInfo *info = FindShip(a_Player, a_Ship);
	assert(0 != info);
	return *info;
}

ShipPlacementInfo* BattleShipsBoard::FindShip(EPlayer a_Player, EShip a_Ship)
{
	ShipPlacementInfo *info = 0;
	ShipPlacementVector::iterator pos = std::find_if(m_Ships.begin(), m_Ships.end(), ShipPredicate(a_Player, a_Ship));
	if (pos != m_Ships.end())
	{
		info = *pos;
	}
	return info;
}

ShipPlacementInfo* BattleShipsBoard::FindCollidingShip(const ShipPlacementInfo &a_Info)
{
	ShipPlacementInfo *info = 0;
	const uint32_t shipSize = GetShipSize(a_Info.m_Ship);
	uint32_t startIndex = a_Info.m_X + a_Info.m_Y * m_Width;
	for (uint32_t i = 0; i < shipSize; ++i)
	{
		if (!IsEmpty(m_Tiles[startIndex]))
		{
			const char tileValue = m_Tiles[startIndex].m_Value;
			uint32_t shipIndex = tileValue - 65;
			ShipPlacementVector::iterator pos = std::find_if(m_Ships.begin(), m_Ships.end(), ShipIndexPredicate(shipIndex));
			info = *pos;
			break;
		}
		startIndex = NextIndex(startIndex, a_Info.m_Direction, m_Width);
	}
	return info;
}

void BattleShipsBoard::ClearMine(const ShipPlacementInfo &a_Info)
{
	const uint32_t shipSize = GetShipSize(a_Info.m_Ship);
	uint32_t startIndex = a_Info.m_X + a_Info.m_Y * m_Width;
	for (uint32_t i = 0; i < shipSize; ++i)
	{
		if (IsMine(m_Tiles[startIndex]))
		{
			ClearTile(m_Tiles[startIndex]);
		}
		startIndex = NextIndex(startIndex, a_Info.m_Direction, m_Width);
	}
}