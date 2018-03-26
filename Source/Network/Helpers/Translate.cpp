#include "Translate.h"

#include <algorithm>
#include <string.h>

namespace
{
	const std::string g_BattleShipsName("BATTLESHIPS");
	const std::string g_ScotlandYardName("SCOTLANDYARD");
	const std::string g_TronName("TRON");
	const std::string g_SokobanName("SOKOBAN");
}

std::string Translate(EGame a_Game)
{
	std::string translation = "";

	switch (a_Game)
	{
	case EGame_BattleShips:
		{
			translation = g_BattleShipsName;
			break;
		}
	case EGame_ScotlandYard:
		{
			translation = g_ScotlandYardName;
			break;
		}
	case EGame_Tron:
		{
			translation = g_TronName;
			break;
		}
	case EGame_Sokoban:
		{
			translation = g_SokobanName;
			break;
		}
	case EGame_Count:
	case EGame_Invalid:
	default:
		{
			translation = "Unknown game.";
			break;
		}
	}
	return translation;
}

EGame Translate(const std::string &a_GameName)
{
	EGame game = EGame_Invalid;
	std::string gameName(a_GameName);
	std::transform(gameName.begin(), gameName.end(), gameName.begin(), ::toupper);

	if (0 == strcmp(g_BattleShipsName.c_str(), gameName.c_str()))
	{
		game = EGame_BattleShips;
	}
	else if (0 == strcmp(g_ScotlandYardName.c_str(), gameName.c_str()))
	{
		game = EGame_ScotlandYard;
	}
	else if (0 == strcmp(g_TronName.c_str(), gameName.c_str()))
	{
		game = EGame_Tron;
	}
	else if (0 == strcmp(g_SokobanName.c_str(), gameName.c_str()))
	{
		game = EGame_Sokoban;
	}
	return game;
}

std::string Translate(EPlayer a_Player)
{
	std::string translation;
	switch (a_Player)
	{
	case EPlayer_One:
		{
			translation = "One";
			break;
		}
	case EPlayer_Two:
		{
			translation = "Two";
			break;
		}
	case EPlayer_Three:
		{
			translation = "Three";
			break;
		}
	case EPlayer_Four:
		{
			translation = "Four";
			break;
		}
	case EPlayer_Five:
		{
			translation = "Five";
			break;
		}
	case EPlayer_Six:
		{
			translation = "Six";
			break;
		}
	case EPlayer_Count:
	case EPlayer_Invalid:
	default:
		{
			translation = "Unknown player.";
			break;
		}
	}
	return translation;
}
