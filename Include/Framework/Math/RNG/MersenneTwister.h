#pragma once

#include "IRandomNumberGenerator.h"
#include "mtrand.h"

#include <stdint.h>

class MersenneTwister : public IRandomNumberGenerator
{
public:
    MersenneTwister(unsigned long a_Seed = 0);
    virtual ~MersenneTwister();
    
    void Seed(unsigned long a_Seed);

	virtual uint16_t NextUint16();
	virtual uint16_t NextUint16(uint16_t a_Min, uint16_t a_Max);
	virtual uint32_t NextUint32();
	virtual uint32_t NextUint32(uint32_t a_Min, uint32_t a_Max);
	virtual uint64_t NextUint64();
	virtual uint64_t NextUint64(uint64_t a_Min, uint64_t a_Max);
	virtual float NextFloat();
	virtual float NextFloat(float a_Min, float a_Max);
	virtual double NextDouble();
	virtual double NextDouble(double a_Min, double a_Max);

private:
    template<typename TYPE>
    TYPE Next();

	template<typename TYPE>
	TYPE Next(TYPE &a_Min, TYPE &a_Max);

	MTRand_closed* m_Generator;
};

template<typename TYPE>
TYPE MersenneTwister::Next()
{
	return static_cast<TYPE>(NextDouble());
}

template<typename TYPE>
TYPE MersenneTwister::Next(TYPE &a_Min, TYPE &a_Max)
{
	return static_cast<TYPE>(NextDouble() * (a_Max - a_Min) + a_Min);
}
