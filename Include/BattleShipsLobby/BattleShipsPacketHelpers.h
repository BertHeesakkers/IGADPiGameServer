#pragma once

#include "Network/RakNetIncludes.h"
#include "BattleShips/EShip.h"

#include <stdint.h>

struct ShipPlacement;

void RetrieveShipPlacementData(RakNet::Packet &a_Packet, ShipPlacement &a_ShipPlacement);
void RetrieveShipRemovalData(RakNet::Packet &a_Packet, EShip &a_Ship);
void RetrieveShootData(RakNet::Packet &a_Packet, uint32_t &a_X, uint32_t &a_Y);
void RetrieveMoveData(RakNet::Packet &a_Packet, EShip &a_Ship, bool &a_Forward);
void RetrieveRotateData(RakNet::Packet &a_Packet, EShip &a_Ship, bool &a_Starboard);
void RetrieveDoubleShotData(RakNet::Packet &a_Packet, uint32_t (&a_X)[2], uint32_t (&a_Y)[2]);
void RetrieveDoubleMoveData(RakNet::Packet &a_Packet, EShip (&a_Ship)[2], bool (&a_Forward)[2]);
void RetrieveMineData(RakNet::Packet &a_Packet, uint32_t &a_X, uint32_t &a_Y);
void RetrieveRadarData(RakNet::Packet &a_Packet, uint32_t &a_X, uint32_t &a_Y);
void RetrieveShieldData(RakNet::Packet &a_Packet, EShip &a_Ship);
void RetrieveDoubleRotateData(RakNet::Packet &a_Packet, EShip (&a_Ship)[2], bool (&a_Starboard)[2]);
void RetrieveRepairData(RakNet::Packet &a_Packet, EShip &a_Ship);
