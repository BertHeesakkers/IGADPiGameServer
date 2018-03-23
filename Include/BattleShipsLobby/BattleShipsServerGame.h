#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Server/BaseServerGame.h"

#include <string>

class BattleShipsGame;
class ILogger;

class BattleShipsServerGame : public BaseServerGame
{
public:
	BattleShipsServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger);
	~BattleShipsServerGame();

	static uint32_t GetNumPlayersPerGame();
	virtual ClientID WhoseTurnIsIt() const override;

	virtual bool HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID) override;

protected:
	virtual std::string GetGameName() const override;
	virtual void AddPlayer() override;

private:
	void HandleStartGame(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleStopGame(RakNet::Packet &a_Packet, ClientID a_ClientID);

	void HandleGetBoard(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandlePlaceShip(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleRemoveShip(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleFinishedShipPlacement(RakNet::Packet &a_Packet, ClientID a_ClientID);

	void HandleShoot(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleMoveShip(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleRotateShip(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleDoubleShot(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleDoubleMove(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleMine(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleRadar(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleShieldShip(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleDoubleRotate(RakNet::Packet &a_Packet, ClientID a_ClientID);
	void HandleRepairShip(RakNet::Packet &a_Packet, ClientID a_ClientID);

	void SendGameStartedMessage();
	void SendOpponentMoveFinishedMessage(ClientID a_ClientID);
	void HandleGameOver();

	BattleShipsGame *m_Game = nullptr;
};