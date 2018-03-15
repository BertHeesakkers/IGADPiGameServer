#include "CreateRandomNumberGenerator.h"
#include "MersenneTwister.h"

AutoPointer<IRandomNumberGenerator> CreateRandomNumberGenerator()
{
	return AutoPointer<IRandomNumberGenerator>(new MersenneTwister());
}

AutoPointer<IRandomNumberGenerator> CreateRandomNumberGenerator(unsigned long a_Seed)
{
	return AutoPointer<IRandomNumberGenerator>(new MersenneTwister(a_Seed));
}
