#include "GameHelpers.h"

#include "Framework/FileHelpers.h"

namespace
{
	const std::string g_ScotlandYardGameHelpFile = "data/scotlandyard_help.txt";
	const std::string g_TronGameHelpFile = "data/tron_help.txt";
	const std::string g_SokobanGameHelpFile = "data/sokoban_help.txt";
}

const std::string GetGameHelp(EGame a_Game)
{
	std::string helpFilename;
	switch (a_Game)
	{
	case EGame_ScotlandYard:
		{
			helpFilename = g_ScotlandYardGameHelpFile;
			break;
		}
	case EGame_Tron:
		{
			helpFilename = g_TronGameHelpFile;
			break;
		}
	case EGame_Sokoban:
		{
			helpFilename = g_SokobanGameHelpFile;
			break;
		}
	case EGame_Count:
	case EGame_Invalid:
	default:
		{
			// do nothing
			break;
		}
	}

	std::string helpContents = "Invalid game code passed. No help available!";
	if (!helpFilename.empty())
	{
		helpContents = ReadFileToString(helpFilename);
	}
	return helpContents;
}
