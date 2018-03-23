#pragma once

#include "Network/RakNetIncludes.h"
#include "Server/BaseLobby.h"

#include <stdint.h>

class ILogger;

class BattleShipsLobby : public BaseLobby
{
public:
	BattleShipsLobby(ILogger &a_Logger);
	virtual ~BattleShipsLobby();

	virtual uint32_t GetNumPlayersPerGame() const override;

protected:
	virtual IServerGame* CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface);
};
