#include "SokobanLobby.h"

#include "SokobanServerGame.h"

SokobanLobby::SokobanLobby(ILogger &a_Logger)
	: BaseLobby(EGame_Sokoban, a_Logger)
{
}

SokobanLobby::~SokobanLobby()
{
}

uint32_t SokobanLobby::GetNumPlayersPerGame() const
{
	return SokobanServerGame::GetNumPlayersPerGame();
}

IServerGame* SokobanLobby::CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface)
{
	SokobanServerGame *game = new SokobanServerGame(a_GameID, a_PeerInterface, m_Logger);
	return game;
}
