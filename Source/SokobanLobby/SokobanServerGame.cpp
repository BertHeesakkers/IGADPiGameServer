#include "SokobanServerGame.h"

#include "Framework/Logging/ILogger.h"
#include "Sokoban/SokobanGame.h"
#include "EMessages.h"
#include "EMessagesExt.h"

namespace
{
	const std::string g_GameName = "Sokoban";
}

SokobanServerGame::SokobanServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger)
	: BaseServerGame(a_GameID, a_PeerInterface, a_Logger)
{
	m_Game = new SokobanGame(a_Logger);
}

SokobanServerGame::~SokobanServerGame()
{
	delete m_Game;
}

uint32_t SokobanServerGame::GetNumPlayersPerGame()
{
	return SokobanGame::GetNumPlayersPerGame();
}

ClientID SokobanServerGame::WhoseTurnIsIt() const
{
	return InvalidClientID();
}

bool SokobanServerGame::HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	//bool messageHandled = false;
	//const unsigned char messageID = static_cast<const unsigned char>(a_Packet.data[0]);
	return false;
}

std::string SokobanServerGame::GetGameName() const
{
	return g_GameName;
}

void SokobanServerGame::AddPlayer()
{
}
