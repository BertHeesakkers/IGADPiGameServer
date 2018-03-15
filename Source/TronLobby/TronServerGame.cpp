#include "TronServerGame.h"

#include "Framework/Logging/ILogger.h"
#include "Tron/TronGame.h"
#include "EMessages.h"
#include "EMessagesExt.h"

namespace
{
	const std::string g_GameName = "Tron";
}

TronServerGame::TronServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger)
	: BaseServerGame(a_GameID, a_PeerInterface, a_Logger)
{
	m_Game = new TronGame(a_Logger);
}

TronServerGame::~TronServerGame()
{
	delete m_Game;
}

uint32_t TronServerGame::GetNumPlayersPerGame()
{
	return TronGame::GetNumPlayersPerGame();
}

ClientID TronServerGame::WhoseTurnIsIt() const
{
	return InvalidClientID();
}

bool TronServerGame::HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	bool messageHandled = false;
	//const unsigned char messageID = static_cast<const unsigned char>(a_Packet.data[0]);
	return messageHandled;
}

std::string TronServerGame::GetGameName() const
{
	return g_GameName;
}

void TronServerGame::AddPlayer()
{
}
