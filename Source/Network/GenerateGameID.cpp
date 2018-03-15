#include "GenerateGameID.h"

#include <stdlib.h>
#include <time.h>
#include <limits>

#include "Framework/AutoPointer.h"
#include "Framework/Math/RNG/CreateRandomNumberGenerator.h"

namespace
{
	AutoPointer<IRandomNumberGenerator> g_RandomNumberGenerator = CreateRandomNumberGenerator(15485863); // millionth prime
}

GameID GenerateGameID()
{
	GameID gameID = g_RandomNumberGenerator->NextUint32();
	return gameID;
}
