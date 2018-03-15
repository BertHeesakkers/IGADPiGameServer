#pragma once

#include "Network/EMessages.h"

enum ETronServerMessages
{
	// game status messages
	EMessage_SendGetGameState,

	EMessage_RecvGameUpdate,
	EMessage_RecvGetGameState
};