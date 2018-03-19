#pragma once

#include "Network/EMessages.h"

enum EScotlandYardServerMessages
{
	// game status messages
	EMessage_SendGetMap = EMessage_BaseCount,
	EMessage_SendGetSpyTravelLog,
	EMessage_SendGetPlayerLocations,
	EMessage_SendTravel,
	EMessage_SendGetLocation,
	EMessage_SendGetTravelLog,
	EMessage_SendAmISpy,
	EMessage_SendGetRemainingTokens,

	EMessage_RecvGetMap,
	EMessage_RecvTravelResult,
	EMessage_RecvGetSpyTravelLog,
	EMessage_RecvGetPlayerLocations,
	EMessage_RecvGetLocation,
	EMessage_RecvAmISpy,
	EMessage_RecvGetTravelLog,
	EMessage_RecvGetRemainingTokens
};
