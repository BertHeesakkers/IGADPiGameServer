#pragma once

#include "EElement.h"

#include <vector>
#include <iostream>

class Level
{
public:
	Level(uint32_t a_Width, uint32_t a_Height);
	~Level();

private:
	uint32_t m_Width;
	uint32_t m_Height;
	EElement *m_Data;

	friend class LevelLoader;
	friend void OutputLevel(const Level&, std::ostream&);
};

typedef std::vector<Level*> LevelSet;
