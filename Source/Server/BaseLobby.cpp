#include "BaseLobby.h"

#include "Framework/AssertMessage.h"
#include "Framework/ClearContainerContents.h"
#include "Framework/Logging/ILogger.h"
#include "Network/Helpers/Translate.h"
#include "Server/IServerGame.h"
#include "UserData.h"

#include <algorithm>

BaseLobby::BaseLobby(EGame a_Game, ILogger &a_Logger)
	: m_Logger(a_Logger)
	, m_Game(a_Game)	
{
}

BaseLobby::~BaseLobby()
{
	ClearContainerContents(m_Games);
}

EGame BaseLobby::GetGameType() const
{
	return m_Game;
}

bool BaseLobby::CanStartNewGame()
{
	const size_t numPlayersWaiting = static_cast<uint32_t>(m_WaitQueue.size());
	const uint32_t numPlayersNeeded = GetNumPlayersPerGame();
	return numPlayersNeeded == numPlayersWaiting;
}

std::vector<IServerGame*>& BaseLobby::GetGames()
{
	return const_cast<std::vector<IServerGame*>&>(const_cast<const BaseLobby*>(this)->GetGames());
}

const std::vector<IServerGame*>& BaseLobby::GetGames() const
{
	return m_Games;
}

void BaseLobby::AddToQueue(UserData &a_UserData)
{
	auto func = [a_UserData](const UserData *userData) { return userData->m_ID == a_UserData.m_ID; };
	auto pos = std::find_if(m_WaitQueue.begin(), m_WaitQueue.end(), func);
	if (pos == m_WaitQueue.end())
	{
		m_WaitQueue.push_back(&a_UserData);
	}

	m_Logger.WriteLine("Client [%s / %d] added to waiting queue for game lobby [%s].", a_UserData.m_Name, a_UserData.m_ClientID, Translate(m_Game));
}

std::vector<UserData*>& BaseLobby::GetWaitQueue()
{
	return m_WaitQueue;
}

void BaseLobby::AddNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface)
{
	m_Logger.WriteLine("AddNewGame(): 1");
	IServerGame *game = CreateNewGame(a_GameID, a_PeerInterface);
	AssertMessage(nullptr != game, "Failed to create new game for lobby!");

	const uint32_t maxNumPlayers = GetNumPlayersPerGame();
	uint32_t playerIndex = 0;
	for (auto pos = m_WaitQueue.begin(); playerIndex < maxNumPlayers && pos != m_WaitQueue.end(); )
	{
		m_Logger.WriteLine("AddNewGame(): 2");
		UserData &userData = **pos;
		pos = m_WaitQueue.erase(pos);
		userData.m_GameID = a_GameID;
		game->SetPlayer(playerIndex, userData);

		m_Logger.WriteLine("New game [%s / %d] starting for client [%s / %d].", Translate(m_Game), a_GameID, userData.m_Name, userData.m_ClientID);
		++playerIndex;
	}
	m_Logger.WriteLine("AddNewGame(): 3");
	game->OpenGameLog();
	m_Logger.WriteLine("AddNewGame(): 4");
	m_Games.push_back(game);
}
