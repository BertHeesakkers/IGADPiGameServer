#pragma once

#include "Base/Network/EMessages.h"

enum EBattleShipsServerMessages
{
	// game status messages
	EMessage_RecvGetBoardData = EMessage_BaseCount,
	EMessage_RecvGameStarted,
	EMessage_RecvGameOver,
	EMessage_RecvGameStopped,

	// game commands
	EMessage_SendStartGame,
	EMessage_SendStopGame,
	EMessage_SendGetBoardData,
	EMessage_SendPlaceShip,
	EMessage_SendRemoveShip,
	EMessage_SendFinishedShipPlacement,
	EMessage_SendShoot,
	EMessage_SendMoveShip,
	EMessage_SendRotateShip,

	// game errors
	EMessage_RecvGameAlreadyStarted,
	EMessage_RecvShipPlacementNotDone,
	EMessage_RecvShipPlacementAlreadyDone,
	EMessage_RecvShipPlacementInvalid,
	EMessage_RecvShipRemovalInvalid,
	EMessage_RecvPlayedBeforeYourTurn
};
