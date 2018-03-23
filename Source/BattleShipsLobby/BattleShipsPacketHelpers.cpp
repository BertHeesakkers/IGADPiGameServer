#include "BattleShipsPacketHelpers.h"

#include "Network/ClientID.h"
#include "Network/GameID.h"
#include "MessageStructures.h"

namespace
{
	void RetrieveShip(RakNet::BitStream &a_BitStream, EShip &a_Ship)
	{
		short ship;
		a_BitStream.Read(ship);
		a_Ship = static_cast<EShip>(ship);
	}
}

void RetrieveShipPlacementData(RakNet::Packet &a_Packet, ShipPlacement &a_ShipPlacement)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	bitStream.Read(a_ShipPlacement.m_Ship);
	bitStream.Read(a_ShipPlacement.m_X);
	bitStream.Read(a_ShipPlacement.m_Y);
	bitStream.Read(a_ShipPlacement.m_Direction);
}

void RetrieveShipRemovalData(RakNet::Packet &a_Packet, EShip &a_Ship)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship);
}

void RetrieveShootData(RakNet::Packet &a_Packet, uint32_t &a_X, uint32_t &a_Y)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	bitStream.Read(a_X);
	bitStream.Read(a_Y);
}

void RetrieveMoveData(RakNet::Packet &a_Packet, EShip &a_Ship, bool &a_Forward)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship);
	bitStream.Read(a_Forward);
}

void RetrieveRotateData(RakNet::Packet &a_Packet, EShip &a_Ship, bool &a_Starboard)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship);
	bitStream.Read(a_Starboard);
}

void RetrieveDoubleShotData(RakNet::Packet &a_Packet, uint32_t (&a_X)[2], uint32_t (&a_Y)[2])
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	bitStream.Read(a_X[0]);
	bitStream.Read(a_Y[0]);
	bitStream.Read(a_X[1]);
	bitStream.Read(a_Y[1]);
}

void RetrieveDoubleMoveData(RakNet::Packet &a_Packet, EShip (&a_Ship)[2], bool (&a_Forward)[2])
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship[0]);
	bitStream.Read(a_Forward[0]);
	RetrieveShip(bitStream, a_Ship[1]);
	bitStream.Read(a_Forward[1]);
}

void RetrieveMineData(RakNet::Packet &a_Packet, uint32_t &a_X, uint32_t &a_Y)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	bitStream.Read(a_X);
	bitStream.Read(a_Y);
}

void RetrieveRadarData(RakNet::Packet &a_Packet, uint32_t &a_X, uint32_t &a_Y)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	bitStream.Read(a_X);
	bitStream.Read(a_Y);
}

void RetrieveShieldData(RakNet::Packet &a_Packet, EShip &a_Ship)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship);
}

void RetrieveDoubleRotateData(RakNet::Packet &a_Packet, EShip (&a_Ship)[2], bool (&a_Starboard)[2])
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship[0]);
	bitStream.Read(a_Starboard[0]);
	RetrieveShip(bitStream, a_Ship[1]);
	bitStream.Read(a_Starboard[1]);
}

void RetrieveRepairData(RakNet::Packet &a_Packet, EShip &a_Ship)
{
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.IgnoreBytes(sizeof(ClientID));
	bitStream.IgnoreBytes(sizeof(GameID));
	RetrieveShip(bitStream, a_Ship);
}
