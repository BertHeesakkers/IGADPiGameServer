#pragma once

#include "Network/GameID.h"

#include <vector>

struct GameData
{
	GameID m_GameID = InvalidGameID();
};

typedef std::vector<GameData*> GameDataVector;
