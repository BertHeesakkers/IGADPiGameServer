#pragma once

#include "ETravelOption.h"
#include <cstdint>

class Node;

class Edge
{
public:
	Edge(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption);
	~Edge();

private:
	const uint32_t m_Start;
	const uint32_t m_End;
	const ETravelOption m_TravelOption;

	friend class Map;
	friend class ScotlandYardGame;
};