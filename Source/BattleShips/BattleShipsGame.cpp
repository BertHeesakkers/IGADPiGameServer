#include "BattleShipsGame.h"

#include "Framework/AssertMessage.h"
#include "BattleShipsBoard.h"
#include "BoardData.h"

#include <iostream>

namespace
{
	const uint32_t g_BoardWidth = 10;
	const uint32_t g_BoardHeight = 20;

	EDirection FlipDirection(EDirection a_Direction)
	{
		EDirection direction = EDirection_Invalid;

		switch (a_Direction)
		{
		case EDirection_Left:
			{
				direction = EDirection_Right;
			}
			break;
		case EDirection_Right:
			{
				direction = EDirection_Left;
			}
			break;
		case EDirection_Up:
			{
				direction = EDirection_Down;
			}
			break;
		case EDirection_Down:
			{
				direction = EDirection_Up;
			}
			break;
		case EDirection_Count:
		case EDirection_Invalid:
		default:
			{
				AssertMessage("Invalid direction encountered!");
				break;
			}
		}

		return direction;
	}

	uint32_t FlipX(uint32_t a_X)
	{
		return (g_BoardWidth - a_X - 1);
	}

	uint32_t FlipY(uint32_t a_Y)
	{
		return (g_BoardHeight - a_Y - 1);
	}
}

BattleShipsGame::BattleShipsGame()
	: m_IsActive(false)
	, m_CurrentPlayer(EPlayer_Invalid)
	, m_Board(0)
	, m_DebugMode(false)
{
	m_Board = new BattleShipsBoard(g_BoardWidth, g_BoardHeight);
	m_PlacementDone[EPlayer_One] = false;
	m_PlacementDone[EPlayer_Two] = false;
}

BattleShipsGame::~BattleShipsGame()
{
	delete m_Board;
}

void BattleShipsGame::Start()
{
	AssertMessage(ShipPlacementDone(), "Attempt to start a game while ship placement has not been finalized!");
	const uint32_t player = rand() % 2;
	m_CurrentPlayer = static_cast<EPlayer>(player);
	m_IsActive = true;
}

void BattleShipsGame::Start(EPlayer a_Player)
{
	AssertMessage(ShipPlacementDone(), "Attempt to start a game while ship placement has not been finalized!");
	m_CurrentPlayer = a_Player;
	m_IsActive = true;
}

bool BattleShipsGame::IsActive() const
{
	return m_IsActive;
}

EPlayer BattleShipsGame::WhoseTurnIsIt() const
{
	return m_CurrentPlayer;
}

bool BattleShipsGame::ShipPlacementDone() const
{
	return m_PlacementDone[static_cast<uint32_t>(EPlayer_One)] && 
		m_PlacementDone[static_cast<uint32_t>(EPlayer_Two)];
}

void BattleShipsGame::GetBoardData(EPlayer a_Player, BoardData &a_BoardData) const
{
	AssertMessage(nullptr != m_Board, "Attempt to retrieve board data without a valid board!");
	a_BoardData.m_Items.clear();
	m_Board->GetData(a_Player, a_BoardData);
	if (EPlayer_Two == a_Player)
	{
		for (std::vector<BoardDataItem>::iterator pos = a_BoardData.m_Items.begin(); pos != a_BoardData.m_Items.end(); ++pos)
		{
			// for player two, flip ship's x and y coordinate and direction
			BoardDataItem &boardDataItem = *pos;
			boardDataItem.m_Direction = FlipDirection(boardDataItem.m_Direction);
			boardDataItem.m_X = FlipX(boardDataItem.m_X);
			boardDataItem.m_Y = FlipY(boardDataItem.m_Y);
		}
	}
}

bool BattleShipsGame::HasShipsLeft(EPlayer a_Player) const
{
	AssertMessage(nullptr != m_Board, "Attempt to check for ships without a valid board!");
	return m_Board->HasShipsLeft(a_Player);
}

bool BattleShipsGame::IsValidPosition(EPlayer a_Player, uint32_t a_X, uint32_t a_Y)
{
	AssertMessage(nullptr != m_Board, "Attempt to check a position without a valid board!");
	bool success = false;
	if (EPlayer_Two == a_Player)
	{
		success = m_Board->IsValidPosition(FlipX(a_X), FlipY(a_Y));
	}
	else if (EPlayer_One == a_Player)
	{
		// for player one, keep the board as is
		success = m_Board->IsValidPosition(a_X, a_Y);
	}
	return success;
}

ECollision BattleShipsGame::PlaceShip(EPlayer a_Player, EShip a_Ship, uint32_t a_X, uint32_t a_Y, EDirection a_Direction)
{
	AssertMessage(nullptr != m_Board, "Attempt to place a ship without a valid board!");
	AssertMessage(!m_PlacementDone[static_cast<uint32_t>(a_Player)], "Attempt to place a ship while the player finalized ship placement already!");
	AssertMessage(a_X < g_BoardWidth && a_Y < g_BoardHeight << 2, "Attempt to place a ship on an invalid position!");
	ECollision collision = ECollision_None;
	if (EPlayer_Two == a_Player)
	{
		// for player two, turn the board upside-down (flip x and y coordinate and direction)
		collision = m_Board->AddShip(a_Player, a_Ship, FlipX(a_X), FlipY(a_Y), FlipDirection(a_Direction));
	}
	else if (EPlayer_One == a_Player)
	{
		// for player one, keep the board as is
		collision = m_Board->AddShip(a_Player, a_Ship, a_X, a_Y, a_Direction);
	}
	return collision;	
}

bool BattleShipsGame::RemoveShip(EPlayer a_Player, EShip a_Ship)
{
	AssertMessage(nullptr != m_Board, "Attempt to remove a ship without a valid board!");
	AssertMessage(!m_PlacementDone[static_cast<uint32_t>(a_Player)], "Attempt to remove a ship while the player finalized ship placement already!");
	return m_Board->RemoveShip(a_Player, a_Ship);
}

void BattleShipsGame::FinishedShipPlacement(EPlayer a_Player)
{
	m_PlacementDone[static_cast<uint32_t>(a_Player)] = true;
}

ECollision BattleShipsGame::Shoot(EPlayer a_Player, uint32_t a_X, uint32_t a_Y, bool &a_SunkShip)
{
	AssertMessage(nullptr != m_Board, "Attempt to shoot without a valid board!");
	ECollision collision = ECollision_None;
	if (a_Player == WhoseTurnIsIt() || m_DebugMode)
	{
		if (a_X < g_BoardWidth && a_Y < g_BoardHeight)
		{
			const uint32_t x = (EPlayer_One == a_Player ? FlipX(a_X) : a_X);
			const uint32_t y = (EPlayer_One == a_Player ? FlipY(a_Y) : a_Y);
			collision = m_Board->Shoot(x, y);
			if (collision != ECollision_None)
			{
				a_SunkShip = m_Board->UpdateShips();
			}
		}
		UpdateCurrentPlayer();
	}
	return collision;
}

ECollision BattleShipsGame::MoveShip(EPlayer a_Player, EShip a_Ship, bool a_Forward)
{
	AssertMessage(nullptr != m_Board, "Attempt to mova a ship without a valid board!");
	ECollision collision = ECollision_None;
	if (a_Player == WhoseTurnIsIt() || m_DebugMode)
	{
		collision = m_Board->MoveShip(a_Player, a_Ship, a_Forward);
		if (collision != ECollision_None)
		{
			m_Board->UpdateShips();
		}
		UpdateCurrentPlayer();
	}
	return collision;
}

ECollision BattleShipsGame::RotateShip(EPlayer a_Player, EShip a_Ship, bool a_Starboard)
{
	AssertMessage(nullptr != m_Board, "Attempt to rotate a ship without a valid board!");
	ECollision collision = ECollision_None;
	if (a_Player == WhoseTurnIsIt() || m_DebugMode)
	{
		collision = m_Board->RotateShip(a_Player, a_Ship, a_Starboard);
		if (collision != ECollision_None)
		{
			m_Board->UpdateShips();
		}
		UpdateCurrentPlayer();
	}
	return collision;
}

void BattleShipsGame::DoubleShot(EPlayer a_Player, uint32_t a_X[2], uint32_t a_Y[2], ECollision (&a_Result)[2], bool (&a_SunkShip)[2])
{
	AssertMessage(nullptr != m_Board, "Attempt to do a double shot without a valid board!");
	ECollision collision = ECollision_None;
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_DoubleShot)) || m_DebugMode)
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			if (a_X[i] < g_BoardWidth && a_Y[i] < g_BoardHeight)
			{
				const uint32_t x = (EPlayer_One == a_Player ? FlipX(a_X[i]) : a_X[i]);
				const uint32_t y = (EPlayer_One == a_Player ? FlipY(a_Y[i]) : a_Y[i]);
				collision = m_Board->Shoot(x, y);
				bool sunkShip = false;
				if (collision != ECollision_None)
				{
					sunkShip = m_Board->UpdateShips();
				}
				a_Result[i] = collision;
				a_SunkShip[i] = sunkShip;
			}
		}
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::DoubleMove(EPlayer a_Player, EShip a_Ship[2], bool a_Forward[2], ECollision(&a_Result)[2])
{
	AssertMessage(nullptr != m_Board, "Attempt to do a double move without a valid board!");
	ECollision collision = ECollision_None;
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_DoubleMove)) || m_DebugMode)
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			collision = m_Board->MoveShip(a_Player, a_Ship[i], a_Forward[i]);
			if (collision != ECollision_None)
			{
				m_Board->UpdateShips();
			}
			a_Result[i] = collision;
		}
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::DoubleRotate(EPlayer a_Player, EShip a_Ship[2], bool a_Starboard[2], ECollision(&a_Result)[2])
{
	AssertMessage(nullptr != m_Board, "Attempt to do a double rotate without a valid board!");
	ECollision collision = ECollision_None;
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_DoubleRotate)) || m_DebugMode)
	{
		for (uint32_t i = 0; i < 2; ++i)
		{
			collision = m_Board->RotateShip(a_Player, a_Ship[i], a_Starboard[i]);
			if (collision != ECollision_None)
			{
				m_Board->UpdateShips();
			}
			a_Result[i] = collision;
		}
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::Mine(EPlayer a_Player, uint32_t a_X, uint32_t a_Y)
{
	AssertMessage(nullptr != m_Board, "Attempt to place a mine without a valid board!");
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_Mine)) || m_DebugMode)
	{
		if (a_X < g_BoardWidth && a_Y < g_BoardHeight)
		{
			const uint32_t x = (EPlayer_One == a_Player ? FlipX(a_X) : a_X);
			const uint32_t y = (EPlayer_One == a_Player ? FlipY(a_Y) : a_Y);
			// check if mine was placed at a valid location
			m_Board->PlaceMine(x, y);
		}
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::Radar(EPlayer a_Player, uint32_t a_X, uint32_t a_Y, char (&a_RadarInfo)[9])
{
	AssertMessage(nullptr != m_Board, "Attempt to use the radar without a valid board!");
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_Radar)) || m_DebugMode)
	{
		if (a_X < g_BoardWidth && a_Y < g_BoardHeight)
		{
			const uint32_t x = (EPlayer_One == a_Player ? FlipX(a_X) : a_X);
			const uint32_t y = (EPlayer_One == a_Player ? FlipY(a_Y) : a_Y);
			m_Board->Radar(x, y, a_RadarInfo);
		}
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::ShieldShip(EPlayer a_Player, EShip a_Ship)
{
	AssertMessage(nullptr != m_Board, "Attempt to shield a ship without a valid board!");
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_ShieldShip)) || m_DebugMode)
	{
		m_Board->ShieldShip(a_Player, a_Ship);
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::RepairShip(EPlayer a_Player, EShip a_Ship)
{
	AssertMessage(nullptr != m_Board, "Attempt to repair a ship without a valid board!");
	if ((a_Player == WhoseTurnIsIt() && !HasCooldownLeft(ECooldownCommand_RepairShip)) || m_DebugMode)
	{
		m_Board->RepairShip(a_Player, a_Ship);
		UpdateCurrentPlayer();
	}
}

void BattleShipsGame::PrintBoard(EPlayer a_Player)
{
	AssertMessage(nullptr != m_Board, "Attempt to print the board without a valid board!");
	AssertMessage(nullptr != m_Board->m_Tiles, "Attempt to print the board without valid tiles on the board!");

	const uint32_t startX = EPlayer_One == a_Player ? 0 : m_Board->m_Width - 1;
	const uint32_t incrementX = EPlayer_One == a_Player ? 1 : -1;
	const uint32_t startY = EPlayer_One == a_Player ? 0 : m_Board->m_Height - 1;
	const uint32_t incrementY = EPlayer_One == a_Player ? 1 : -1;
	const std::string playerTopTitle = EPlayer_One == a_Player ? "Player one" : "Player two";
	const std::string playerBottomTitle = EPlayer_One == a_Player ? "Player two" : "Player one";

	uint32_t lineIndex = 0;

	std::cout << "    -----------------------------------------" << std::endl;
	std::cout << "                    " << playerTopTitle.c_str() << std::endl;
	std::cout << "    -----------------------------------------" << std::endl;
	std::cout << "      0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9" << std::endl;
	std::cout << "    -----------------------------------------" << std::endl;
	for (uint32_t y = startY; y >= 0 && y < m_Board->m_Height; y = y + incrementY, lineIndex++)
	{
		if (lineIndex < 10)
		{
			std::cout << "  " << lineIndex;
		}
		else
		{
			std::cout << " " << lineIndex;
		}
		for (uint32_t x = startX; x < m_Board->m_Width; x = x + incrementX)
		{
			const uint32_t index = y * m_Board->m_Width + x;
			std::cout << " | " << static_cast<char>(m_Board->m_Tiles[index].m_Value);
		}
		std::cout << " | " << (m_Board->m_Height - lineIndex - 1);
		std::cout << std::endl << "    -----------------------------------------";
		std::cout << std::endl;
	}
	std::cout << "      9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0" << std::endl;
	std::cout << "    -----------------------------------------" << std::endl;
	std::cout << "                    " << playerBottomTitle.c_str() << std::endl;
	std::cout << "    -----------------------------------------" << std::endl;
}

void BattleShipsGame::SetDebugMode()
{
	m_DebugMode = !m_DebugMode;
}

uint32_t BattleShipsGame::GetNumPlayersPerGame()
{
	return 2;
}

void BattleShipsGame::UpdateCurrentPlayer()
{
	const uint32_t currentPlayer = static_cast<uint32_t>(m_CurrentPlayer);
	for (uint32_t cooldownIndex = 0; cooldownIndex < static_cast<uint32_t>(ECooldownCommand_Count); ++cooldownIndex)
	{
		if (m_CurrentCooldowns[currentPlayer][cooldownIndex] > 0)
		{
			m_CurrentCooldowns[currentPlayer][cooldownIndex]--;
		}
	}
	m_CurrentPlayer = (m_CurrentPlayer == EPlayer_One) ? EPlayer_Two : EPlayer_One;
}

bool BattleShipsGame::HasCooldownLeft(ECooldownCommand a_Command) const
{
	return 0 != m_CurrentCooldowns[static_cast<uint32_t>(a_Command)];
}
