#pragma once

enum EServerError
{
	EServerError_NoError,
	EServerError_GeneralError,

	EServerError_InvalidPassword,
	EServerError_NoPassword,
	EServerError_NotLoggedIn,

	EServerError_UserDataUnavailable,
	EServerError_GameLobbyUnavailable,

	EServerError_InvalidGameCommand,
	EServerError_InvalidGameID,

	EServerError_Count,
	EServerError_Invalid
};