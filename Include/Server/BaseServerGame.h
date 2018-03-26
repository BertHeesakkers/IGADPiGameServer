#pragma once

#include "Network/ClientID.h"
#include "Network/EPlayer.h"
#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Network/WriteBitStream.h"
#include "IServerGame.h"
#include "UserData.h"

#include <vector>

class ILogger;
struct UserData;

class BaseServerGame : public IServerGame
{
public:
	BaseServerGame(GameID a_Game, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger);
	virtual ~BaseServerGame();

	virtual GameID GetGameID() const override;
	virtual void SetPlayer(uint32_t a_PlayerIndex, UserData &a_UserData) override;
	virtual const UserDataVector& GetPlayers() const override;

protected:
	virtual void AddPlayer() = 0;
	EPlayer GetPlayer(ClientID a_ClientID) const;
	ClientID GetClient(EPlayer a_Player) const;

	RakNet::RakPeerInterface& GetPeerInterface();
	ILogger& GetGameLogger();
	ILogger& GetServerLogger();

	void BroadcastTurnFinished(const ClientID &a_ClientID) const;

private:
	virtual void OpenGameLog();

	const GameID m_GameID;
	ILogger &m_ServerLogger;
	ILogger *m_GameLogger = nullptr;
	RakNet::RakPeerInterface &m_PeerInterface;
	std::vector<UserData*> m_Players;
};