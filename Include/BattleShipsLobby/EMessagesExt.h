#pragma once

enum BattleShipsServerMessagesExt
{
	EMessage_SendDoubleMove = 0xB6,
	EMessage_SendMine = 0xC8,
	EMessage_SendDoubleShot = 0xC9,
	EMessage_SendDoubleRotate = 0x8B,
	EMessage_SendRadar = 0xD6,
	EMessage_SendRepairShip = 0xCE,
	EMessage_SendShieldShip = 0xE0
};
