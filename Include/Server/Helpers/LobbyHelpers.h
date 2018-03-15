#pragma once

#include "Server/EGame.h"

#include <vector>

class ILobby;
class ILogger;

ILobby* FindGameLobby(const std::vector<ILobby*> &m_Lobbies, EGame a_Game);
ILobby* CreateGameLobby(EGame a_Game, ILogger &a_Logger);