#include "TronLobby.h"

#include "TronServerGame.h"

TronLobby::TronLobby(ILogger &a_Logger)
	: BaseLobby(EGame_Tron, a_Logger)
{
}

TronLobby::~TronLobby()
{
}

uint32_t TronLobby::GetNumPlayersPerGame() const
{
	return TronServerGame::GetNumPlayersPerGame();
}

IServerGame* TronLobby::CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface)
{
	TronServerGame *game = new TronServerGame(a_GameID, a_PeerInterface, m_Logger);
	return game;
}
