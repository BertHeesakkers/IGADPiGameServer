#include "SokobanGame.h"

#include "LevelLoader.h"

#include "Framework/Logging/ILogger.h"
#include "Framework/Math/RNG/CreateRandomNumberGenerator.h"
#include "Framework/Math/RNG/IRandomNumberGenerator.h"
#include "Framework/AutoPointer.h"
#include "Framework/ClearContainerContents.h"

#include <assert.h>

namespace
{
	AutoPointer<IRandomNumberGenerator> g_RandomNumberGenerator = CreateRandomNumberGenerator(982451653);	// http://primes.utm.edu/lists/small/millions/

	const std::vector<std::string> GetPredefinedLevelSets()
	{
		std::vector<std::string> levelSets;
		levelSets.push_back("Data\\FourthSetExtended.txt");
		levelSets.push_back("Data\\JPKLevels.txt");
		levelSets.push_back("Data\\JPKLevelsArrangements.txt");
		levelSets.push_back("Data\\KobusTheronCollection.txt");
		levelSets.push_back("Data\\MacTommyInventions.txt");
		levelSets.push_back("Data\\OriginalRemixed.txt");
		levelSets.push_back("Data\\SimpleSokoban.txt");
		levelSets.push_back("Data\\ThinkingRabbitSet.txt");
		levelSets.push_back("Data\\XSokoban.txt");
		return levelSets;
	}
}

SokobanGame::SokobanGame(ILogger &a_Logger)
	: m_Logger(a_Logger)
{
	LoadPredefinedLevelSets();
}

SokobanGame::~SokobanGame()
{
	for (std::map<HashedString, LevelSet*>::iterator pos = m_LevelSets.begin(); pos != m_LevelSets.end(); ++pos)
	{
		LevelSet *levelSet = pos->second;
		ClearContainerContents(*levelSet);
		delete levelSet;
	}
}

const Level& SokobanGame::GetGameLevel()
{
	const std::vector<std::string> levelSets = GetPredefinedLevelSets();
	const uint32_t numLevelSets = static_cast<uint32_t>(levelSets.size());

	const uint32_t selectedLevelSetIndex = g_RandomNumberGenerator->NextUint32(0, numLevelSets);
	const LevelSet &levelSet = GetLevelSet(levelSets[selectedLevelSetIndex]);

	const uint32_t numLevels = static_cast<uint32_t>(levelSet.size());
	const uint32_t selectedLevelIndex = g_RandomNumberGenerator->NextUint32(0, numLevels);

	const Level* level = levelSet[selectedLevelIndex];
	return *level;
}

uint32_t SokobanGame::GetNumPlayersPerGame()
{
	return 2;
}

void SokobanGame::LoadPredefinedLevelSets()
{
	const std::vector<std::string> levelSets = GetPredefinedLevelSets();
	for (std::vector<std::string>::const_iterator pos = levelSets.begin(); pos != levelSets.end(); ++pos)
	{
		const std::string filename = *pos;
		LoadLevelSet(filename);
	}
}

void SokobanGame::LoadLevelSet(const std::string &a_Filename)
{
	m_Logger.WriteLine("[Game] attempt to load level set (%s).", a_Filename.c_str());

	LevelSet *levelSet = new LevelSet();
	LevelLoader loader(a_Filename, *levelSet);
	assert(!levelSet->empty());

	m_LevelSets.insert(std::make_pair(HashedString(a_Filename), levelSet));
}

const LevelSet& SokobanGame::GetLevelSet(const HashedString &a_LevelSet)
{
#if defined(_DEBUG)
	m_Logger.WriteLine("[Game] attempt to retrieve map set (%s).", a_LevelSet.AsCString());
#else
	m_Logger.WriteLine("[Game] attempt to retrieve map set (%d).", a_LevelSet.GetHash());
#endif

	LevelSet *levelSet = 0;
	std::map<HashedString, LevelSet*>::iterator pos = m_LevelSets.find(a_LevelSet);
	if (m_LevelSets.end() != pos)
	{
		levelSet = (pos->second);
	}
	assert(0 != levelSet);
	return *levelSet;
}

const Level& SokobanGame::GetLevel(const HashedString &a_LevelSet, uint32_t a_Index)
{
#if defined(_DEBUG)
	m_Logger.WriteLine("[Game] attempt to retrieve map (%d) from set (%s).", a_Index, a_LevelSet.AsCString());
#else
	m_Logger.WriteLine("[Game] attempt to retrieve map (%d) from set (%d).", a_Index, a_LevelSet.GetHash());
#endif

	Level *level = 0;
	std::map<HashedString, LevelSet*>::iterator pos = m_LevelSets.find(a_LevelSet);
	if (m_LevelSets.end() != pos)
	{
		LevelSet &levelSet = *(pos->second);
		if (a_Index < static_cast<uint32_t>(levelSet.size()))
		{
			level = levelSet[a_Index];
		}
	}
	assert(0 != level);
	return *level;
}
