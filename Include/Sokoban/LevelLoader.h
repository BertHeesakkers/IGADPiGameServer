#pragma once

#include "EElement.h"
#include "Level.h"

#include <fstream>
#include <map>
#include <string>
#include <vector>

class LevelLoader
{
public:
	LevelLoader(const std::string &a_Filename, LevelSet &a_LevelSet);
	~LevelLoader();

private:
	typedef std::pair<uint32_t, uint32_t> Coordinate;
	typedef std::map<Coordinate, EElement> LevelDefinition;
	LevelDefinition levelBlocks;

	void LoadLevels(LevelSet &a_LevelSet);
	Level* CreateLevel(const LevelDefinition &a_LevelBlocks, uint32_t a_MaxWidth, uint32_t a_MaxHeigth);

	std::ifstream m_LevelFile;
	LevelSet m_LevelSet;
};

