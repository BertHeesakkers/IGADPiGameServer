#pragma once

#include "IRandomNumberGenerator.h"
#include "../../AutoPointer.h"

AutoPointer<IRandomNumberGenerator> CreateRandomNumberGenerator();
AutoPointer<IRandomNumberGenerator> CreateRandomNumberGenerator(unsigned long a_Seed);
