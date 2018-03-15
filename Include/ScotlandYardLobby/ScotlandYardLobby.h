#pragma once

#include "Server/BaseLobby.h"

class ILogger;

class ScotlandYardLobby : public BaseLobby
{
public:
	ScotlandYardLobby(ILogger &a_Logger);
	virtual ~ScotlandYardLobby();

	virtual uint32_t GetNumPlayersPerGame() const override;

protected:
	virtual IServerGame* CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface);
};