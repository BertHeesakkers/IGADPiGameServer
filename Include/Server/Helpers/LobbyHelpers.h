#pragma once
#include "Network/GameID.h"
#include "Server/EGame.h"

#include <vector>

class ILobby;
class ILogger;
struct UserData;

ILobby* FindGameLobby(const std::vector<ILobby*> &m_Lobbies, EGame a_Game);
ILobby* FindGameLobby(const std::vector<ILobby*> &m_Lobbies, GameID a_GameID);
ILobby* CreateGameLobby(EGame a_Game, ILogger &a_Logger);