#pragma once

#include <stdint.h>

class IRandomNumberGenerator
{
public:
    virtual ~IRandomNumberGenerator() { }
    virtual void Seed(unsigned long a_Seed) = 0;
    
	virtual uint16_t NextUint16() = 0;
	virtual uint16_t NextUint16(uint16_t a_Min, uint16_t a_Max) = 0;
	virtual uint64_t NextUint64() = 0;
	virtual uint64_t NextUint64(uint64_t a_Min, uint64_t a_Max) = 0;
	virtual uint32_t NextUint32() = 0;
	virtual uint32_t NextUint32(uint32_t a_Min, uint32_t a_Max) = 0;
	virtual float NextFloat() = 0;
    virtual float NextFloat(float a_Min, float a_Max) = 0;
	virtual double NextDouble() = 0;
	virtual double NextDouble(double a_Min, double a_Max) = 0;
};
