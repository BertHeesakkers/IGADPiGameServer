#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Server/BaseServerGame.h"

#include <string>

class SokobanGame;
class ILogger;

class SokobanServerGame : public BaseServerGame
{
public:
	SokobanServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger);
	~SokobanServerGame();

	static uint32_t GetNumPlayersPerGame();
	virtual ClientID WhoseTurnIsIt() const override;

	virtual bool HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID) override;

protected:
	virtual std::string GetGameName() const override;
	virtual void AddPlayer() override;

private:
	SokobanGame *m_Game = nullptr;
};