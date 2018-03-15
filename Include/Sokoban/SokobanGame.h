#pragma once

#include "Framework/HashedString.h"
#include "Level.h"

#include <map>
#include <string>

class ILogger;

class SokobanGame
{
public:
	SokobanGame(ILogger &a_Logger);
	~SokobanGame();

	const Level& GetGameLevel();

	static uint32_t GetNumPlayersPerGame();

private:
	void LoadPredefinedLevelSets();
	void LoadLevelSet(const std::string &a_Filename);
	const LevelSet& GetLevelSet(const HashedString &a_LevelSet);
	const Level& GetLevel(const HashedString &a_LevelSet, uint32_t a_Index);

	ILogger &m_Logger;
	std::map<HashedString, LevelSet*> m_LevelSets;
};
