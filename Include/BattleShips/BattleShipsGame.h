#pragma once

#include "Network/EPlayer.h"
#include "ECollision.h"
#include "EDirection.h"
#include "EShip.h"

#include <stdint.h>
#include <vector>

class BattleShipsBoard;

struct BoardData;

class BattleShipsGame
{
public:
	BattleShipsGame();
	~BattleShipsGame();

	void Start();
	void Start(EPlayer a_Player);
	bool IsActive() const;

	EPlayer WhoseTurnIsIt() const;
	bool ShipPlacementDone() const;
	void GetBoardData(EPlayer a_Player, BoardData &a_BoardData) const;
	bool HasShipsLeft(EPlayer a_Player) const;

	bool IsValidPosition(EPlayer a_Player, uint32_t a_X, uint32_t a_Y);
	ECollision PlaceShip(EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction);
	bool RemoveShip(EPlayer a_Player, EShip a_Ship);
	void FinishedShipPlacement(EPlayer a_Player);

	ECollision Shoot(EPlayer a_Player, uint32_t a_X, uint32_t a_Y, bool &a_SunkShip);
	ECollision MoveShip(EPlayer a_Player, EShip a_Ship, bool a_Forward);
	ECollision RotateShip(EPlayer a_Player, EShip a_Ship, bool a_Starboard);
	void DoubleShot(EPlayer a_Player, uint32_t a_X[2], uint32_t a_Y[2], ECollision (&a_Result)[2], bool (&a_SunkShip)[2]);
	void DoubleMove(EPlayer a_Player, EShip a_Ship[2], bool a_Forward[2], ECollision (&a_Result)[2]);
	void DoubleRotate(EPlayer a_Player, EShip a_Ship[2], bool a_Starboard[2], ECollision (&a_Result)[2]);
	void Mine(EPlayer a_Player, uint32_t a_X, uint32_t a_Y);
	void Radar(EPlayer a_Player, uint32_t a_X, uint32_t a_Y, char (&a_RadarInfo)[9]);
	void ShieldShip(EPlayer a_Player, EShip a_Ship);
	void RepairShip(EPlayer a_Player, EShip a_Ship);

	void PrintBoard(EPlayer a_Player);
	void SetDebugMode();

	static uint32_t GetNumPlayersPerGame();

private:
	enum ECooldownCommand
	{
		ECooldownCommand_DoubleShot,
		ECooldownCommand_DoubleMove,
		ECooldownCommand_DoubleRotate,
		ECooldownCommand_Mine,
		ECooldownCommand_Radar,
		ECooldownCommand_ShieldShip,
		ECooldownCommand_RepairShip,

		ECooldownCommand_Count
	};

	void UpdateCurrentPlayer();
	bool HasCooldownLeft(ECooldownCommand a_Command) const;

	const short m_Cooldowns[ECooldownCommand_Count] = { 3, 5, 3, 3, 3, 5, 5 };
	short m_CurrentCooldowns[2][ECooldownCommand_Count] = { { 0, 0, 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0, 0, 0 } };

	bool m_IsActive;
	bool m_PlacementDone[EPlayer_Count];
	EPlayer m_CurrentPlayer;
	BattleShipsBoard *m_Board;

	bool m_DebugMode;
};
