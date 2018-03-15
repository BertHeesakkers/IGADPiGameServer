#include "LevelLoader.h"

#include "Framework/AssertMessage.h"
#include "Framework/ClearContainerContents.h"

#include <algorithm>
#include <assert.h>
#include <map>

namespace
{
	EElement GetElement(const char a_Value)
	{
		EElement element = EElement_Invalid;

		switch (a_Value)
		{
		case '@':
			{
				element = EElement_Player;
			}
			break;
		case '.':
			{
				element = EElement_GoalSquare;
			}
			break;
		case '#':
			{
				element = EElement_Wall;
			}
			break;
		case '$':
			{
				element = EElement_Box;
			}
			break;
		case '*':
			{
				element = EElement_BoxOnGoalSquare;
			}
			break;
		case '+':
			{
				element = EElement_PlayerOnGoalSquare;
			}
			break;
		case ' ':
			{
				element = EElement_Floor;
			}
			break;
		case '\t':
			{
				// white space; ignore
			}
			break;
		default:
			{
				assert(false);
			}
			break;
		}

		AssertMessage(EElement_Invalid != element, "Invalid element encountered in level!");
		return element;
	}
}

LevelLoader::LevelLoader(const std::string &a_Filename, LevelSet &a_LevelSet)
	: m_LevelFile(a_Filename)
{
	assert(m_LevelFile.is_open());
	LoadLevels(a_LevelSet);
}

LevelLoader::~LevelLoader()
{
	ClearContainerContents(m_LevelSet);
}

void LevelLoader::LoadLevels(LevelSet &a_LevelSet)
{
	std::pair<LevelDefinition::iterator, bool> result;

	bool parsingLevel = false;
	uint32_t rowNumber = 0;
	//uint32_t columnNumber = 0;
	uint32_t maxColumn = 0;
	std::size_t found;
	std::string line;
	while (std::getline(m_LevelFile, line))
	{
		found = line.find_first_of("#");
		parsingLevel = (found != std::string::npos) ? true : false;
		parsingLevel = parsingLevel && (line[0] == ' ' || line[0] == '#');
		if (parsingLevel)
		{
			const uint32_t stringLength = static_cast<uint32_t>(line.size());
			maxColumn = std::max(maxColumn, stringLength);
			for (uint32_t index = 0; index < stringLength; ++index)
			{
				EElement element = GetElement(line[index]);
#if defined(_DEBUG)
				result = levelBlocks.insert(std::make_pair(std::make_pair(index, rowNumber), element));
				assert(result.second);
#else
				levelBlocks.insert(std::make_pair(std::make_pair(index, rowNumber), element));
#endif
			}
			++rowNumber;
		}
		else if (!levelBlocks.empty())
		{
			Level *level = CreateLevel(levelBlocks, maxColumn, rowNumber);
			a_LevelSet.push_back(level);

			rowNumber = 0;
			//columnNumber = 0;
			maxColumn = 0;
			levelBlocks.clear();
		}
	}
}

Level* LevelLoader::CreateLevel(const LevelDefinition &a_LevelBlocks, uint32_t a_MaxWidth, uint32_t a_MaxHeigth)
{
	Level *level = new Level(a_MaxWidth, a_MaxHeigth);
	const uint32_t maxIndex = level->m_Width * level->m_Height;
	for (uint32_t index = 0; index < maxIndex; ++index)
	{
		level->m_Data[index] = EElement_Floor;
	}

	for (LevelDefinition::const_iterator pos = a_LevelBlocks.begin(); pos != a_LevelBlocks.end(); ++pos)
	{
		const Coordinate &coordinate = pos->first;
		EElement element = pos->second;
		const uint32_t index = coordinate.first + coordinate.second * a_MaxWidth;
		assert(index < maxIndex);
		level->m_Data[coordinate.first + coordinate.second * a_MaxWidth] = element;
	}
	return level;
}
