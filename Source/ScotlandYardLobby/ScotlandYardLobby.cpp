#include "ScotlandYardLobby.h"

#include "Framework/Logging/ILogger.h"
#include "ScotlandYardServerGame.h"

ScotlandYardLobby::ScotlandYardLobby(ILogger &a_Logger)
	: BaseLobby(EGame_ScotlandYard, a_Logger)
{
}

ScotlandYardLobby::~ScotlandYardLobby()
{
}

uint32_t ScotlandYardLobby::GetNumPlayersPerGame() const
{
	return ScotlandYardServerGame::GetNumPlayersPerGame();
}

IServerGame* ScotlandYardLobby::CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface)
{
	ScotlandYardServerGame *game = new ScotlandYardServerGame(a_GameID, a_PeerInterface, m_Logger);
	return game;
}
