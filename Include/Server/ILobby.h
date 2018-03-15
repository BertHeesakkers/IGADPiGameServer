#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"

#include <stdint.h>
#include <vector>

class IServerGame;
struct UserData;

class ILobby
{
public:
	virtual EGame GetGameType() const = 0;
	virtual uint32_t GetNumPlayersPerGame() const = 0;
	virtual bool CanStartNewGame() = 0;
	virtual void AddNewGame(GameID a_Game, RakNet::RakPeerInterface &a_PeerInterface) = 0;
	virtual const std::vector<IServerGame*>& GetGames() const = 0;
	virtual std::vector<IServerGame*>& GetGames() = 0;

	virtual void AddToQueue(UserData &a_UserData) = 0;
	virtual std::vector<UserData*>& GetWaitQueue() = 0;

protected:
	virtual IServerGame* CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface) = 0;
};
