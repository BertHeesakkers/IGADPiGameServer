#pragma once

#include "RakNetIncludes.h"

enum EMessages
{
	EMessage_SendServerHelp = ID_USER_PACKET_ENUM + 1,
	EMessage_SendGameHelp,	
	EMessage_SendLogin,
	EMessage_SendDisconnect,
	EMessage_SendJoinGame,
	EMessage_SendWhoseTurnIsIt,

	EMessage_RecvServerHelp,
	EMessage_RecvGameHelp,
	EMessage_RecvLoginSuccess,
	EMessage_RecvDisconnected,
	EMessage_RecvWaitingForPlayers,
	EMessage_RecvGameJoined,
	EMessage_RecvWhoseTurnIsIt,
	EMessage_RecvOpponentFinished,
	EMessage_RecvAcknowledgement,
	EMessage_RecvServerError,
	EMessage_RecvGameNotActive,
	// See https://github.com/BertHeesakkers/IGADPiGameServer/issues/13 for below
	EMessage_SendLobbyData,
	EMessage_RecvLobbyData,
	// End
	/**
	* Added by Simon Renger
	* Date: 27-03-2018
	* Description:  Have added them because EMessage_RecvDisconnected gets send if a player left the game
	* They get send of a player left the lobby or dissconected.
	**/
	EMessage_RecvPlayerLeftLobby,
	EMessage_RecvPlayerLeftGame,
	EMessage_Count,
	EMessage_BaseCount = EMessage_Count
};
