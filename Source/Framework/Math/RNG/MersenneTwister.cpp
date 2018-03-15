#include "MersenneTwister.h"

#include <limits>

MersenneTwister::MersenneTwister(unsigned long a_Seed /* = 0 */)
{
    m_Generator = new MTRand_closed(a_Seed);
}

MersenneTwister::~MersenneTwister()
{
    delete m_Generator;
}

void MersenneTwister::Seed(unsigned long a_Seed)
{
    m_Generator->seed(a_Seed);
}

uint16_t MersenneTwister::NextUint16()
{
	return static_cast<uint16_t>(NextUint32(std::numeric_limits<uint16_t>::min(), std::numeric_limits<uint16_t>::max()));
}

uint16_t MersenneTwister::NextUint16(uint16_t a_Min, uint16_t a_Max)
{
	return Next<uint16_t>(a_Min, a_Max);
}

uint32_t MersenneTwister::NextUint32()
{
	return NextUint32(std::numeric_limits<uint32_t>::min(), std::numeric_limits<uint32_t>::max());
}

uint32_t MersenneTwister::NextUint32(uint32_t a_Min, uint32_t a_Max)
{
	return Next<uint32_t>(a_Min, a_Max);
}

uint64_t MersenneTwister::NextUint64()
{
	return NextUint64(std::numeric_limits<uint64_t>::min(), std::numeric_limits<uint64_t>::max());
}

uint64_t MersenneTwister::NextUint64(uint64_t a_Min, uint64_t a_Max)
{
	return Next<uint64_t>(a_Min, a_Max);
}

float MersenneTwister::NextFloat()
{
	return Next<float>();
}

float MersenneTwister::NextFloat(float a_Min, float a_Max)
{
	return Next<float>(a_Min, a_Max);
}

double MersenneTwister::NextDouble()
{
	return m_Generator->operator()();
}

double MersenneTwister::NextDouble(double a_Min, double a_Max)
{
	return NextDouble() * (a_Max - a_Min) + a_Min;
}
