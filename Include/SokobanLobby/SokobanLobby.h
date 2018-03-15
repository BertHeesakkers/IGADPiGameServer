#pragma once

#include "Server/BaseLobby.h"

class ILogger;

class SokobanLobby : public BaseLobby
{
public:
	SokobanLobby(ILogger &a_Logger);
	virtual ~SokobanLobby();

	virtual uint32_t GetNumPlayersPerGame() const override;

protected:
	virtual IServerGame* CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface);
};