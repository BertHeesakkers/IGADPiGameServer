#pragma once

#include "Network/EMessages.h"

enum EScotlandYardServerMessages
{
	// game status messages
	EMessage_SendGetMap = EMessage_BaseCount,
	EMessage_SendGetSpyTravelLog,
	EMessage_SendGetPlayerLocations,
	EMessage_SendTravel,
	EMessage_SendGetLocation,			// to do
	EMessage_SendGetTravelLog,			// to do
	EMessage_SendAmISpy,				// to do
	EMessage_SendGetRemainingTokens,	// to do

	EMessage_RecvGetMap,
	EMessage_RecvTravelResult,
	EMessage_RecvGetSpyTravelLog,
	EMessage_RecvGetPlayerLocations,
	EMessage_RecvGetLocation,			// to do
	EMessage_RecvAmISpy,				// to do
	EMessage_RecvGetTravelLog,			// to do
	EMessage_RecvGetRemainingTokens		// to do
};
