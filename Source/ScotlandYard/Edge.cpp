#include "Edge.h"

Edge::Edge(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption)
	: m_Start(a_Start)
	, m_End(a_End)
	, m_TravelOption(a_TravelOption)
{
}

Edge::~Edge()
{
}
