#include "GenerateClientID.h"

#include <stdlib.h>
#include <time.h>
#include <limits>

#include "Framework/AutoPointer.h"
#include "Framework/Math/RNG/CreateRandomNumberGenerator.h"

namespace
{
	AutoPointer<IRandomNumberGenerator> g_RandomNumberGenerator = CreateRandomNumberGenerator(15485863); // millionth prime
}

ClientID GenerateClientID()
{
	ClientID clientID = g_RandomNumberGenerator->NextUint32();
	return clientID;
}
