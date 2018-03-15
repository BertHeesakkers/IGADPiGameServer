#pragma once

#include "Server/BaseLobby.h"

class ILogger;

class TronLobby : public BaseLobby
{
public:
	TronLobby(ILogger &a_Logger);
	virtual ~TronLobby();

	virtual uint32_t GetNumPlayersPerGame() const override;

protected:
	virtual IServerGame* CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface);
};
