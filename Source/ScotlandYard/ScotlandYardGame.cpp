#include "ScotlandYardGame.h"

#include "Framework/AssertMessage.h"
#include "Framework/AutoPointer.h"
#include "Framework/Math/RNG/CreateRandomNumberGenerator.h"
#include "Framework/StringFunctions.h"
#include "Detective.h"
#include "Edge.h"
#include "Node.h"
#include "Ply.h"
#include "Spy.h"

#include <algorithm>
#include <fstream>
#include <vector>

namespace
{
	const std::string g_ScotlandYardMapFile = "data/ScotlandYard/map_nodes.txt";
	static const std::vector<int> g_StartLocations = { 13, 26, 29, 34, 50, 53, 91, 94, 103, 112, 117, 132, 138, 141, 155, 174, 197, 198 };

	AutoPointer<IRandomNumberGenerator> g_RandomNumberGenerator = CreateRandomNumberGenerator(15485863); // millionth prime

	uint32_t StringToInt(const std::string &a_String)
	{
		std::stringstream convert(a_String);
		uint32_t result;
		convert >> result;
		return result;
	}

	void InsertEdges(Map &a_Map, const uint32_t a_StartIndex, std::vector<std::string> &a_Destinations, ETravelOption a_TravelOption)
	{
		for (auto pos = a_Destinations.begin(); pos != a_Destinations.end(); ++pos)
		{
			const uint32_t destination = StringToInt(*pos);
			a_Map.AddEdge(a_StartIndex, destination, a_TravelOption);
		}
	}

	bool IsPositionFree(uint32_t a_Position, const std::vector<Player*> &a_Players, bool a_IgnoreSpy = false)
	{
		bool positionFree = true;
		for (auto pos = a_Players.begin(); pos != a_Players.end() && positionFree; ++pos)
		{
			const Player &player = **pos;
			positionFree = player.GetPosition() != a_Position;
			if (player.IsSpy())
			{
				positionFree = true;
			}
		}
		return positionFree;
	}

	bool IsAssigned(const std::vector<Player*> &a_Players, EPlayer a_Player)
	{
		bool assigned = false;
		const uint32_t numPlayers = static_cast<uint32_t>(a_Players.size());
		for (uint32_t playerIndex = 0; playerIndex < numPlayers; ++playerIndex)
		{
			const Player &player = *a_Players[playerIndex];
			if (a_Player == player.GetPlayer())
			{
				assigned = true;
			}
		}
		return assigned;
	}

	void DetermineUnassignedPlayers(const std::vector<Player*> &a_Players, std::vector<uint32_t> &a_UnassignedPlayers)
	{
		const uint32_t numPlayers = static_cast<uint32_t>(a_Players.size());
		for (uint32_t playerIndex = 0; playerIndex < numPlayers; ++playerIndex)
		{
			const Player &player = *a_Players[playerIndex];
			if (EPlayer_Invalid == player.GetPlayer())
			{
				a_UnassignedPlayers.push_back(playerIndex);
			}
		}
	}

	bool HasCaughtSpy(const Player &a_Player, const Spy &a_Spy)
	{
		return a_Player.GetPosition() == a_Spy.GetPosition();
	}
}

ScotlandYardGame::ScotlandYardGame()
	: m_NumPlayers(ScotlandYardGame::GetNumPlayersPerGame())
{
	m_Players.push_back(new Spy());
	for (uint32_t index = 1; index < m_NumPlayers; ++index)
	{
		m_Players.push_back(new Detective());
	}
	LoadMap(g_ScotlandYardMapFile);
}

ScotlandYardGame::~ScotlandYardGame()
{
}

void ScotlandYardGame::AddPlayer(EPlayer a_Player)
{
	AssertMessage(!IsAssigned(m_Players, a_Player), "Player is already part of this game!");
	AssertMessage(static_cast<uint32_t>(a_Player) < m_NumPlayers, "Invalid player index encountered!");
	uint32_t startNodeIndex = 0;
	do 
	{
		const uint32_t startIndex = g_RandomNumberGenerator->NextUint32(0, 18);
		startNodeIndex = g_StartLocations[startIndex];
	} while (!IsPositionFree(startNodeIndex, m_Players));

	std::vector<uint32_t> freePlayers;
	DetermineUnassignedPlayers(m_Players, freePlayers);

	const uint32_t numUnassisgnedPlayers = static_cast<uint32_t>(freePlayers.size());
	if (numUnassisgnedPlayers > 0)
	{
		const uint32_t playerIndex = g_RandomNumberGenerator->NextUint32(0, numUnassisgnedPlayers);

		Player &player = *m_Players[freePlayers[playerIndex]];
		player.m_Player = a_Player;
		player.m_Position = startNodeIndex;

		if (GetNumPlayersPerGame() == GetNumPlayers())
		{
			Start();
		}
	}
}

const Player& ScotlandYardGame::GetPlayer(EPlayer a_Player) const
{
	return const_cast<Player&>(const_cast<ScotlandYardGame*>(this)->GetPlayer(a_Player));
}

bool ScotlandYardGame::IsSpy(EPlayer a_Player)
{
	AssertMessage(static_cast<uint32_t>(a_Player) < m_NumPlayers, "Invalid player index encountered!");
	Player &player = GetPlayer(a_Player);
	return player.IsSpy();
}

ETravelResult ScotlandYardGame::Travel(EPlayer a_Player, uint32_t a_Destination, ETravelOption a_TravelOption)
{
	ETravelResult travelResult = ETravelResult_Success;
	if (IsPositionFree(a_Destination, m_Players, true))
	{
		Player &player = GetPlayer(a_Player);
		const bool hasTokens = player.HasTokens(a_TravelOption);
		if (hasTokens)
		{
			const uint32_t startIndex = player.GetPosition();
			const bool canTravel = m_Map.CanTravel(startIndex, a_Destination, a_TravelOption);
			if (canTravel)
			{
				player.m_Position = a_Destination;
				player.DropToken(a_TravelOption);
				if (!player.IsSpy())
				{
					GetSpy().AddToken(a_TravelOption);
				}
				player.UpdateTravelLog(startIndex, a_Destination, a_TravelOption);

				if (!player.IsSpy() && HasCaughtSpy(player, GetSpy()))
				{
					travelResult = ETravelResult_CaughtSpy;
					m_GameOver = true;
				}

				m_Ply->FinishedTurn(a_Player);
			}
			else
			{
				travelResult = ETravelResult_NoValidTravelOption;
			}
		}
		else
		{
			travelResult = ETravelResult_NoTokensLeft;
		}
	}
	else
	{
		travelResult = ETravelResult_DestinationNotFree;
	}
	return travelResult;
}

bool ScotlandYardGame::IsActive() const
{
	return m_IsActive;
}

EPlayer ScotlandYardGame::WhoseTurnIsIt() const
{
	return m_Ply->GetCurrentPlayer();
}

const TravelLog ScotlandYardGame::GetSpyTravelLog() const
{
	TravelLog travelLog;
	ConvertToSpyLog(GetSpy().GetTravelLog(), travelLog);
	return travelLog;
}

uint32_t ScotlandYardGame::GetNumPlayers() const
{
	std::vector<uint32_t> unassignedPlayers;
	DetermineUnassignedPlayers(m_Players, unassignedPlayers);
	return GetNumPlayersPerGame() - static_cast<uint32_t>(unassignedPlayers.size());
}

uint32_t ScotlandYardGame::GetNumPlayersPerGame()
{
	return 6;
}

const std::string& ScotlandYardGame::GetMapFilename()
{
	return g_ScotlandYardMapFile;
}

void ScotlandYardGame::Start()
{
	std::vector<uint32_t> freePlayers;
	DetermineUnassignedPlayers(m_Players, freePlayers);
	if (freePlayers.empty())
	{
		std::vector<EPlayer> turnOrder;
		turnOrder.push_back(GetSpy().GetPlayer());
		for (uint32_t index = 0; index < m_NumPlayers; ++index)
		{
			Player &player = *m_Players[index];
			if (!player.IsSpy())
			{
				turnOrder.push_back(player.GetPlayer());
			}
		}
		m_Ply = new Ply(turnOrder);
	}
	m_IsActive = true;
}

void ScotlandYardGame::LoadMap(const std::string &a_Filename)
{
	std::ifstream fileStream(a_Filename.c_str());
	AssertMessage(fileStream.is_open(), "Unable to open legacy map file!");

	// Scotland Yard map has 200 nodes
	// Lines in file are structured:
	// Node index; Taxi destinations; Bus destinations; Subway destinations; Ferry destinations; Coordinates; Name (if applicable)

	Map m_Map;

	std::string line;
	std::vector<std::string> temp;
	while (std::getline(fileStream, line))
	{
		if (!line.empty())
		{
			std::vector<std::string> tokens = Tokenize(line, ";");
			uint32_t nodeIndex = StringToInt(tokens[0]);
			Node *node = m_Map.m_Nodes[nodeIndex - 1];
			AssertMessage(nullptr != node, "Encounter uninitialized node!");

			temp = Tokenize(tokens[1], ",");
			InsertEdges(m_Map, nodeIndex, temp, ETravelOption_Taxi);
			temp = Tokenize(tokens[2], ",");
			InsertEdges(m_Map, nodeIndex, temp, ETravelOption_Bus);
			temp = Tokenize(tokens[3], ",");
			InsertEdges(m_Map, nodeIndex, temp, ETravelOption_Underground);
			temp = Tokenize(tokens[4], ",");
			InsertEdges(m_Map, nodeIndex, temp, ETravelOption_Ferry);
			temp = Tokenize(tokens[5], ",");
			node->m_X = StringToInt(temp[0]);
			node->m_Y = StringToInt(temp[1]);
		}
	}
}

Player* ScotlandYardGame::FindPlayer(EPlayer a_Player)
{
	Player *player = nullptr;
	for (auto pos = m_Players.begin(); pos != m_Players.end(); ++pos)
	{
		if (a_Player == (**pos).m_Player)
		{
			player = *pos;
			break;
		}
	}
	return player;
}

Player& ScotlandYardGame::GetPlayer(EPlayer a_Player)
{
	Player *player = FindPlayer(a_Player);
	AssertMessage(nullptr != player, "Unable to find requested player!");
	return *player;
}

Spy* ScotlandYardGame::FindSpy()
{
	return const_cast<Spy*>(const_cast<const ScotlandYardGame*>(this)->FindSpy());
}

const Spy* ScotlandYardGame::FindSpy() const
{
	Spy *spy = nullptr;
	for (auto pos = m_Players.begin(); pos != m_Players.end(); ++pos)
	{
		Player &player = **pos;
		if (player.IsSpy())
		{
			spy = dynamic_cast<Spy*>(&player);
		}
	}
	return spy;
}

Spy& ScotlandYardGame::GetSpy()
{
	return const_cast<Spy&>(static_cast<const ScotlandYardGame*>(this)->GetSpy());
}

const Spy& ScotlandYardGame::GetSpy() const
{
	const Spy *spy = FindSpy();
	AssertMessage(nullptr != spy, "Unable to find spy!");
	return *spy;
}
