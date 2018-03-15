#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Server/BaseServerGame.h"

#include <string>

class TronGame;
class ILogger;

class TronServerGame : public BaseServerGame
{
public:
	TronServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger);
	~TronServerGame();

	static uint32_t GetNumPlayersPerGame();
	virtual ClientID WhoseTurnIsIt() const override;

	virtual bool HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID) override;

protected:
	virtual std::string GetGameName() const override;
	virtual void AddPlayer() override;

private:
	TronGame *m_Game = nullptr;
};