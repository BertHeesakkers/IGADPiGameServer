#include "LobbyHelpers.h"

#include "BattleShipsLobby/BattleShipsLobby.h"
#include "Framework/AssertMessage.h"
#include "ScotlandYardLobby/ScotlandYardLobby.h"
#include "Server/ILobby.h"
#include "Server/IServerGame.h"
#include "SokobanLobby/SokobanLobby.h"
#include "TronLobby/TronLobby.h"
#include "Server/UserData.h"
#include <algorithm>

ILobby* FindGameLobby(const std::vector<ILobby*> &m_Lobbies, EGame a_Game)
{
	ILobby *lobby = nullptr;
	auto pos = std::find_if(m_Lobbies.begin(), m_Lobbies.end(), [a_Game](const ILobby *a_Lobby) { return a_Lobby->GetGameType() == a_Game; });
	if (m_Lobbies.end() != pos)
	{
		lobby = *pos;
	}
	return lobby;
}
ILobby* FindGameLobby(const std::vector<ILobby*> &m_Lobbies, GameID a_GameID)
{
	for(auto* lobby:m_Lobbies){
		for(auto* game : lobby->GetGames()){
				if(game->GetGameID() == a_GameID)
					return lobby;
			}
	}
	return nullptr;
}
ILobby* CreateGameLobby(EGame a_Game, ILogger &a_Logger)
{
	ILobby *lobby = nullptr;
	switch (a_Game)
	{
	case EGame_BattleShips:
		{
			lobby = new BattleShipsLobby(a_Logger);
			break;
		}
	case EGame_ScotlandYard:
		{
			lobby = new ScotlandYardLobby(a_Logger);
			break;
		}
	case EGame_Tron:
		{
			lobby = new TronLobby(a_Logger);
			break;
		}
	case EGame_Sokoban:
		{
			lobby = new SokobanLobby(a_Logger);
			break;
		}
	case EGame_Count:
	case EGame_Invalid:
	default:
		{
			break;
		}
	}
	AssertMessage(nullptr != lobby, "Failed to create lobby for game!");
	return lobby;
}