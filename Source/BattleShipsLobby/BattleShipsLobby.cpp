#include "BattleShipsLobby.h"

#include "BattleShipsServerGame.h"

BattleShipsLobby::BattleShipsLobby(ILogger &a_Logger)
	: BaseLobby(EGame_BattleShips, a_Logger)
{
}

BattleShipsLobby::~BattleShipsLobby()
{
}

uint32_t BattleShipsLobby::GetNumPlayersPerGame() const
{
	return BattleShipsServerGame::GetNumPlayersPerGame();
}

IServerGame* BattleShipsLobby::CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface)
{
	BattleShipsServerGame *game = new BattleShipsServerGame(a_GameID, a_PeerInterface, m_Logger);
	return game;
}
