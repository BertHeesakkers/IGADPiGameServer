#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Server/BaseServerGame.h"

#include <string>

class ScotlandYardGame;
class ILogger;

class ScotlandYardServerGame : public BaseServerGame
{
public:
	ScotlandYardServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger);
	~ScotlandYardServerGame();

	static uint32_t GetNumPlayersPerGame();
	virtual ClientID WhoseTurnIsIt() const override;

	virtual bool HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID) override;

protected:
	virtual std::string GetGameName() const override;
	virtual void AddPlayer() override;

private:
	void HandleGetMap(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleGetSpyTravelLog(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleGetPlayerLocations(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleTravel(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleGetLocation(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleGetTravelLog(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleGetAmISpy(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleWhoIsSpy(RakNet::Packet &a_Packet);
	void HandleGetRemainingTokens(RakNet::Packet &a_Packet, ClientID a_ClientID);

	ScotlandYardGame *m_Game = nullptr;
};