#include "BaseServerGame.h"

#include "Framework/AssertMessage.h"
#include "Framework/Logging/FileLogger.h"
#include "Network/MessageHelpers.h"
#include "Server/UserData.h"

#include <sstream>

namespace
{
	const std::string g_BaseLogDirectory = "log/";
}

BaseServerGame::BaseServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger)
	: m_GameID(a_GameID)
	, m_ServerLogger(a_Logger)
	, m_PeerInterface(a_PeerInterface)
{
}

BaseServerGame::~BaseServerGame()
{
	delete m_GameLogger;
}

GameID BaseServerGame::GetGameID() const
{
	return m_GameID;
}

void BaseServerGame::SetPlayer(uint32_t a_PlayerIndex, UserData &a_UserData)
{
	m_Players.push_back(&a_UserData);
	AddPlayer();
}

EPlayer BaseServerGame::GetPlayer(ClientID a_ClientID) const
{
	EPlayer player = EPlayer_Invalid;
	auto pos = std::find_if(m_Players.begin(), m_Players.end(), [&a_ClientID](const UserData *a_UserData) { return a_UserData->m_ClientID == a_ClientID; });
	if (m_Players.end() != pos)
	{
		player = static_cast<EPlayer>(pos - m_Players.begin());
	}
	return player;
}

const std::vector<UserData*>& BaseServerGame::GetPlayers() const
{
	return m_Players;
}

ClientID BaseServerGame::GetClient(EPlayer a_Player) const
{
	const uint32_t player = static_cast<uint32_t>(a_Player);
	AssertMessage(player < static_cast<uint32_t>(m_Players.size()), "Player index out of bounds for this game!");
	return m_Players[player]->m_ClientID;
}

RakNet::RakPeerInterface& BaseServerGame::GetPeerInterface()
{
	return m_PeerInterface;
}

ILogger& BaseServerGame::GetGameLogger()
{
	AssertMessage(nullptr != m_GameLogger, "Attempt to retrieve an invalid game logger!");
	return *m_GameLogger;
}

void BaseServerGame::BroadcastTurnFinished(const ClientID &a_ClientID) const
{
	for (auto pos = m_Players.begin(); pos != m_Players.end(); ++pos)
	{
		const UserData &userData = **pos;
		if (a_ClientID != userData.m_ClientID)
		{
			RakNet::BitStream payload;
			payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvOpponentFinished));
			SendMessage(m_PeerInterface, userData.m_SystemAddress, payload);
		}
	}
}

void BaseServerGame::OpenGameLog()
{
	std::stringstream logFilename;
	logFilename << g_BaseLogDirectory << GetGameName() << "_" << m_GameID << ".log";
	m_GameLogger = new FileLogger(logFilename.str());
}
