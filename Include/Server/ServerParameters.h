#pragma once

#include "Server/EGame.h"

#include <vector>

struct ServerParameters
{
	unsigned short m_ServerPort = 60000;
	unsigned short m_MaxClients = 512;
	std::vector<EGame> m_Lobbies;
};
