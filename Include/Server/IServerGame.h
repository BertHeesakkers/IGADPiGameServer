#pragma once

#include "Network/ClientID.h"
#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "UserData.h"

#include <string>

class IServerGame
{
public:
	virtual ~IServerGame() { }

	virtual GameID GetGameID() const = 0;

	virtual bool HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID) = 0;
	virtual void SetPlayer(uint32_t a_PlayerIndex, UserData &a_UserData) = 0;
	virtual void OpenGameLog() = 0;
	virtual ClientID WhoseTurnIsIt() const = 0;
	virtual const UserDataVector& GetPlayers() const = 0;

protected:
	virtual std::string GetGameName() const = 0;
};
