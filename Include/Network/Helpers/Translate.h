#pragma once

#include "Network/EPlayer.h"
#include "Server/EGame.h"

#include <string>

std::string Translate(EGame a_Game);
EGame Translate(const std::string &a_GameName);
std::string Translate(EPlayer a_Player);
