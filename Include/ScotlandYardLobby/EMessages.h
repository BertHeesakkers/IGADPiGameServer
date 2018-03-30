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

	// New:
	EMessage_SendWhoIsSpy,
	// End

	EMessage_SendGetRemainingTokens,

	EMessage_RecvGetMap,
	EMessage_RecvTravelResult,
	EMessage_RecvGetSpyTravelLog,
	EMessage_RecvGetPlayerLocations,
	EMessage_RecvGetLocation,
	EMessage_RecvGetTravelLog, // Caution: Order of messages changed to keep consistency with Send messages.
	EMessage_RecvAmISpy,

	// New:
	EMessage_RecvWhoIsSpy,
	// End

	EMessage_RecvGetRemainingTokens
};
