#include "BattleShipsServerGame.h"

#include "Framework/AssertMessage.h"
#include "Framework/Logging/ILogger.h"
#include "Network/EMessages.h"
#include "Network/MessageHelpers.h"
#include "EMessages.h"
#include "EMessagesExt.h"
#include "BattleShips/BattleShipsGame.h"
#include "BattleShips/BoardData.h"
#include "BattleShips/Helpers/GetDirection.h"
#include "MessageStructures.h"
#include "BattleShipsPacketHelpers.h"
#include "Server/UserData.h"

namespace
{
	const std::string g_GameName = "BattleShips";

	void WriteShipDataToBitStream(const BoardDataItem &a_ShipData, RakNet::BitStream &a_BitStream)
	{
		a_BitStream.Write(static_cast<short>(a_ShipData.m_Ship));
		a_BitStream.Write(a_ShipData.m_X);
		a_BitStream.Write(a_ShipData.m_Y);
		a_BitStream.Write(static_cast<short>(a_ShipData.m_Direction));
		const uint32_t shipSize = static_cast<uint32_t>(a_ShipData.m_Damage.size());
		for (uint32_t i = 0; i < shipSize; ++i)
		{
			a_BitStream.Write(a_ShipData.m_Damage[i]);
		}
	}

	void SendAcknowledgementMessage(RakNet::RakPeerInterface &a_PeerInterface, RakNet::SystemAddress a_SystemAddress, ECollision a_Collision, bool a_ShipSunk)
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvAcknowledgement));
		payload.Write(static_cast<short>(a_Collision));
		payload.Write(a_ShipSunk);
		SendNetworkMessage(a_PeerInterface, a_SystemAddress, payload);
	}

	void SendAcknowledgementMessage(RakNet::RakPeerInterface &a_PeerInterface, RakNet::SystemAddress a_SystemAddress, ECollision (&a_Collision)[2], bool (&a_ShipSunk)[2])
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvAcknowledgement));
		payload.Write(static_cast<short>(a_Collision[0]));
		payload.Write(a_ShipSunk[0]);
		payload.Write(static_cast<short>(a_Collision[1]));
		payload.Write(a_ShipSunk[1]);
		SendNetworkMessage(a_PeerInterface, a_SystemAddress, payload);
	}

	void SendAcknowledgementMessage(RakNet::RakPeerInterface &a_PeerInterface, RakNet::SystemAddress a_SystemAddress, ECollision a_Collision)
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvAcknowledgement));
		payload.Write(static_cast<short>(a_Collision));
		SendNetworkMessage(a_PeerInterface, a_SystemAddress, payload);
	}

	void SendAcknowledgementMessage(RakNet::RakPeerInterface &a_PeerInterface, RakNet::SystemAddress a_SystemAddress, ECollision (&a_Collision)[2])
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvAcknowledgement));
		payload.Write(static_cast<short>(a_Collision[0]));
		payload.Write(static_cast<short>(a_Collision[1]));
		SendNetworkMessage(a_PeerInterface, a_SystemAddress, payload);
	}

	void SendAcknowledgementMessage(RakNet::RakPeerInterface &a_PeerInterface, RakNet::SystemAddress a_SystemAddress, char (&a_RadarInfo)[9])
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvAcknowledgement));
		payload.Write(static_cast<short>(a_RadarInfo[0]));
		payload.Write(static_cast<short>(a_RadarInfo[1]));
		payload.Write(static_cast<short>(a_RadarInfo[2]));
		payload.Write(static_cast<short>(a_RadarInfo[3]));
		payload.Write(static_cast<short>(a_RadarInfo[4]));
		payload.Write(static_cast<short>(a_RadarInfo[5]));
		payload.Write(static_cast<short>(a_RadarInfo[6]));
		payload.Write(static_cast<short>(a_RadarInfo[7]));
		payload.Write(static_cast<short>(a_RadarInfo[8]));
		SendNetworkMessage(a_PeerInterface, a_SystemAddress, payload);
	}
}

BattleShipsServerGame::BattleShipsServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger)
	: BaseServerGame(a_GameID, a_PeerInterface, a_Logger)
{
	m_Game = new BattleShipsGame();
}

BattleShipsServerGame::~BattleShipsServerGame()
{
	delete m_Game;
}

uint32_t BattleShipsServerGame::GetNumPlayersPerGame()
{
	return BattleShipsGame::GetNumPlayersPerGame();
}

ClientID BattleShipsServerGame::WhoseTurnIsIt() const
{
	return GetClient(m_Game->WhoseTurnIsIt());
}

bool BattleShipsServerGame::HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	bool messageHandled = false;
	const unsigned char messageID = static_cast<const unsigned char>(a_Packet.data[0]);
	switch (messageID)
	{
	case EMessage_SendGetBoardData:
		{
			HandleGetBoard(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendPlaceShip:
		{
		HandlePlaceShip(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendRemoveShip:
		{
			HandleRemoveShip(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendFinishedShipPlacement:
		{
			HandleFinishedShipPlacement(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendStartGame:
		{
			HandleStartGame(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendStopGame:
		{
			HandleStopGame(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendShoot:
		{
			HandleShoot(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendMoveShip:
		{
			HandleMoveShip(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendRotateShip:
		{
			HandleRotateShip(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendDoubleShot:
		{
			HandleDoubleShot(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendDoubleMove:
		{
			HandleDoubleMove(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendMine:
		{
			HandleMine(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendRadar:
		{
			HandleRadar(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendShieldShip:
		{
			HandleShieldShip(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendDoubleRotate:
		{
			HandleDoubleRotate(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendRepairShip:
		{
			HandleRepairShip(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	default:
		{
			SendServerErrorMessage(GetPeerInterface(), a_Packet.systemAddress, EServerError_InvalidGameCommand);
			break;
		}
	}
	return messageHandled;
}

std::string BattleShipsServerGame::GetGameName() const
{
	return g_GameName;
}

void BattleShipsServerGame::AddPlayer()
{
}

void BattleShipsServerGame::HandleStartGame(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameAlreadyStarted);
	}
	else if (!m_Game->ShipPlacementDone())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipPlacementNotDone);
	}
	else
	{
		m_Game->Start();
		SendGameStartedMessage();
	}
}

void BattleShipsServerGame::HandleStopGame(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const std::vector<UserData*> players = GetPlayers();
		for (auto pos = players.begin(); pos != players.end(); ++pos)
		{
			const UserData &userData = **pos;
			SendNetworkMessage(GetPeerInterface(), userData.m_SystemAddress, EMessage_RecvGameStopped);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleGetBoard(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	GetGameLogger().WriteLine("[BattleShipsGame] GETBOARD %d", a_ClientID);

	RakNet::BitStream payload;
	payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetBoardData));

	BoardData boardData;
	m_Game->GetBoardData(GetPlayer(a_ClientID), boardData);
	for (std::vector<BoardDataItem>::const_iterator pos = boardData.m_Items.begin(); pos != boardData.m_Items.end(); ++pos)
	{
		const BoardDataItem &item = *pos;
		WriteShipDataToBitStream(item, payload);
	}
	SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
}

void BattleShipsServerGame::HandlePlaceShip(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameAlreadyStarted);
	}
	else if (m_Game->ShipPlacementDone())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipPlacementAlreadyDone);
	}
	else
	{
		ShipPlacement shipPlacement;
		RetrieveShipPlacementData(a_Packet, shipPlacement);
		const EPlayer player = GetPlayer(a_ClientID);

		GetGameLogger().WriteLine("[BattleShipsGame] PLACESHIP [%d] [%d] [%d] [%d] [%d]", player, shipPlacement.m_Ship, shipPlacement.m_X, shipPlacement.m_Y, shipPlacement.m_Direction);

		if (m_Game->IsValidPosition(player, shipPlacement.m_X, shipPlacement.m_Y))
		{
			const EShip ship = static_cast<EShip>(shipPlacement.m_Ship);
			const EDirection direction = GetDirection(shipPlacement.m_Direction);
			if (ECollision_None == m_Game->PlaceShip(player, ship, shipPlacement.m_X, shipPlacement.m_Y, direction))
			{
				SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress);
			}
			else
			{
				SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipPlacementInvalid);
			}
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipPlacementInvalid);
		}
	}
}

void BattleShipsServerGame::HandleRemoveShip(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameAlreadyStarted);
	}
	else if (m_Game->ShipPlacementDone())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipPlacementAlreadyDone);
	}
	else
	{
		const EPlayer player = GetPlayer(a_ClientID);
		EShip ship = EShip_Invalid;
		RetrieveShipRemovalData(a_Packet, ship);
		if (m_Game->RemoveShip(player, ship))
		{
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress);
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipRemovalInvalid);
		}
	}
}

void BattleShipsServerGame::HandleFinishedShipPlacement(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	GetGameLogger().WriteLine("[BattleShipsGame] GETBOARD %d", a_ClientID);
	
	if (m_Game->IsActive())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameAlreadyStarted);
	}
	else if (m_Game->ShipPlacementDone())
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvShipPlacementAlreadyDone);
	}
	else
	{
		const EPlayer player = GetPlayer(a_ClientID);
		m_Game->FinishedShipPlacement(player);
		SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress);
	}
}

void BattleShipsServerGame::HandleShoot(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			uint32_t x = 0;
			uint32_t y = 0;
			RetrieveShootData(a_Packet, x, y);

			bool shipSunk = false;
			const ECollision result = m_Game->Shoot(player, x, y, shipSunk);

			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, result, shipSunk);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleMoveShip(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			EShip ship = EShip_Invalid;
			bool forward;
			RetrieveMoveData(a_Packet, ship, forward);
			const ECollision result = m_Game->MoveShip(player, ship, forward);

			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, result);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleRotateShip(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			EShip ship = EShip_Invalid;
			bool starboard;
			RetrieveRotateData(a_Packet, ship, starboard);

			const ECollision result = m_Game->RotateShip(player, ship, starboard);

			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, result);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleDoubleShot(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			uint32_t x[2] = { 0, 0 };
			uint32_t y[2] = { 0, 0 };
			RetrieveDoubleShotData(a_Packet, x, y);
			ECollision result[2] = { ECollision_Invalid,ECollision_Invalid };
			bool shipSunk[2] = { false, false };
			m_Game->DoubleShot(player, x, y, result, shipSunk);
			
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, result, shipSunk);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleDoubleMove(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			EShip ships[2] = { EShip_Invalid, EShip_Invalid };
			bool forward[2] = { false, false };
			RetrieveDoubleMoveData(a_Packet, ships, forward);

			ECollision result[2] = { ECollision_Invalid, ECollision_Invalid };
			m_Game->DoubleMove(player, ships, forward, result);
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, result);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleMine(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			uint32_t x = 0;
			uint32_t y = 0;
			RetrieveMineData(a_Packet, x, y);
			m_Game->Mine(player, x, y);
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleRadar(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			uint32_t x = 0;
			uint32_t y = 0;
			RetrieveRadarData(a_Packet, x, y);
			char radarInfo[9] = {};
			m_Game->Radar(player, x, y, radarInfo);

			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, radarInfo);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleShieldShip(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			EShip ship = EShip_Invalid;
			RetrieveShieldData(a_Packet, ship);
			m_Game->ShieldShip(player, ship);
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleDoubleRotate(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			EShip ships[2] = { EShip_Invalid, EShip_Invalid };
			bool starboard[2] = { false, false };
			RetrieveDoubleRotateData(a_Packet, ships, starboard);

			ECollision result[2] = { ECollision_Invalid, ECollision_Invalid };
			m_Game->DoubleRotate(player, ships, starboard, result);
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress, result);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::HandleRepairShip(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			EShip ship = EShip_Invalid;
			RetrieveRepairData(a_Packet, ship);
			m_Game->RepairShip(player, ship);
			SendAcknowledgementMessage(GetPeerInterface(), a_Packet.systemAddress);
			SendOpponentMoveFinishedMessage(a_ClientID);
			HandleGameOver();
		}
		else
		{
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvPlayedBeforeYourTurn);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void BattleShipsServerGame::SendGameStartedMessage()
{
	const EPlayer startingPlayer = m_Game->WhoseTurnIsIt();
	const ClientID startingClientID = GetClient(startingPlayer);

	RakNet::BitStream payload;
	payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGameStarted));
	payload.Write(startingClientID);
	const std::vector<UserData*> players = GetPlayers();
	for (auto pos = players.begin(); pos != players.end(); ++pos)
	{
		const UserData &userData = **pos;
		SendNetworkMessage(GetPeerInterface(), userData.m_SystemAddress, payload);
	}
}

void BattleShipsServerGame::SendOpponentMoveFinishedMessage(ClientID a_ClientID)
{
	// find other player
	// send message
}

void BattleShipsServerGame::HandleGameOver()
{
	const bool playerOneShipsLeft = m_Game->HasShipsLeft(EPlayer_One);
	const bool playerTwoShipsLeft = m_Game->HasShipsLeft(EPlayer_Two);
	AssertMessage(playerOneShipsLeft || playerTwoShipsLeft, "Both players have no ships left!");

	if (!(playerOneShipsLeft && playerTwoShipsLeft))
	{
		const ClientID winner = playerOneShipsLeft ? GetClient(EPlayer_One) : GetClient(EPlayer_Two);

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGameOver));
		payload.Write(winner);

		const std::vector<UserData*> players = GetPlayers();
		for (auto pos = players.begin(); pos != players.end(); ++pos)
		{
			const UserData &userData = **pos;
			SendNetworkMessage(GetPeerInterface(), userData.m_SystemAddress, payload);
		}
	}
}
