#include "Map.h"

#include "Framework/AssertMessage.h"
#include "Framework/ClearContainerContents.h"
#include "Edge.h"
#include "Node.h"
#include <iostream>

Map::Map()
{
	for (uint32_t i = 0; i < 200; ++i)
		m_Nodes.push_back(new Node(i + 1));
}

Map::~Map()
{
	ClearContainerContents(m_Nodes);
	ClearContainerContents(m_Edges);
}

void Map::AddEdge(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption)
{
	std::cout << "DEBUG: AddEdge " << a_Start << ">" << a_End << " using " << a_TravelOption << std::endl;

	AssertMessage(a_Start <= static_cast<uint32_t>(m_Nodes.size()), "Start index is too large!");
	AssertMessage(a_End <= static_cast<uint32_t>(m_Nodes.size()), "End index is too large!");
	m_Edges.push_back(new Edge(m_Nodes[a_Start - 1]->m_Index, m_Nodes[a_End - 1]->m_Index, a_TravelOption));

	std::cout << "DEBUG: AddEdge end" << std::endl;
}

bool Map::CanTravel(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption) const
{
	for (Edge* edge : m_Edges)
		if (m_Nodes[edge->m_Start - 1]->m_Index == a_Start && m_Nodes[edge->m_End - 1]->m_Index == a_End && edge->m_TravelOption == a_TravelOption)
			return true;
	return false;
}

void Map::GetTransport(uint32_t a_Start, uint32_t a_End, std::vector<ETravelOption> a_TravelOptions) const
{
	for (Edge* edge : m_Edges)
		if (m_Nodes[edge->m_Start - 1]->m_Index == a_Start && m_Nodes[edge->m_End - 1]->m_Index == a_End)
			a_TravelOptions.push_back(edge->m_TravelOption);
}
