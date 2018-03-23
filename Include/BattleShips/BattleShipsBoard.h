#pragma once

#include "Network/EPlayer.h"
#include "ECollision.h"
#include "EDirection.h"
#include "EShip.h"
#include "ShipPlacementInfo.h"

#include <stdint.h>
#include <vector>

struct BoardData;

class BattleShipsBoard
{
public:
	struct Tile
	{
		char m_Value;
	};

public:
	BattleShipsBoard(uint32_t a_Width, uint32_t a_Height);
	~BattleShipsBoard();

	void Clear();
	ECollision AddShip(EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction);
	bool RemoveShip(EPlayer a_Player, EShip a_Ship);

	ECollision MoveShip(EPlayer a_Player, EShip a_Ship, bool a_Forward);
	ECollision RotateShip(EPlayer a_Player, EShip a_Ship, bool a_Starboard);

	bool IsValidPosition(uint32_t a_X, uint32_t a_Y);
	ECollision Shoot(uint32_t a_X, uint32_t a_Y);
	void PlaceMine(uint32_t a_X, uint32_t a_Y);
	void RepairShip(EPlayer a_Player, EShip a_Ship);
	void ShieldShip(EPlayer a_Player, EShip a_Ship);
	void Radar(uint32_t a_X, uint32_t a_Y, char *a_RadarInfo);

	bool UpdateShips();
	void GetData(EPlayer a_Player, BoardData &a_Data);

private:
	static uint32_t ms_AddedShips;

	ECollision CanPlaceShip(EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction);
	bool HasShip(EPlayer a_Player, EShip a_Ship) const;
	bool HasShipsLeft(EPlayer a_Player) const;

	ShipPlacementInfo& GetShip(EPlayer a_Player, EShip a_Ship);
	ShipPlacementInfo* FindShip(EPlayer a_Player, EShip a_Ship);
	ShipPlacementInfo* FindCollidingShip(const ShipPlacementInfo &a_Ship);
	void ClearMine(const ShipPlacementInfo &a_Info);

	uint32_t m_Width;
	uint32_t m_Height;

	Tile *m_Tiles;
	ShipPlacementVector m_Ships;

	friend class BattleShipsGame;
};
