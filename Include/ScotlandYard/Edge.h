#pragma once

#include "ETravelOption.h"

class Node;

class Edge
{
public:
	Edge(Node &a_Start, Node &a_End, ETravelOption a_TravelOption);
	~Edge();

private:
	const Node &m_Start;
	const Node &m_End;
	const ETravelOption m_TravelOption;

	friend class Map;
};