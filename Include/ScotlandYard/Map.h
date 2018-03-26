#pragma once

#include "ETravelOption.h"

#include <stdint.h>
#include <vector>

class Edge;
class Node;

class Map
{
public:
	Map();
	~Map();

	void AddEdge(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption);

	bool CanTravel(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption) const;
	void GetTransport(uint32_t a_Start, uint32_t a_End, std::vector<ETravelOption> a_TravelOptions) const;

private:
	std::vector<Node*> m_Nodes;
	std::vector<Edge*> m_Edges;

	friend class ScotlandYardGame;
	friend class ScotlandYardServerGame;
};