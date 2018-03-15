#include "Node.h"

Node::Node(uint32_t a_Index)
	: m_Index(a_Index)
	, m_X(static_cast<uint32_t>(-1))
	, m_Y(static_cast<uint32_t>(-1))
{
}

Node::~Node()
{
}

uint32_t Node::GetX() const
{
	return m_X;
}

uint32_t Node::GetY() const
{
	return m_Y;
}

void Node::GetCoordinates(uint32_t &a_X, uint32_t &a_Y)
{
	a_X = m_X;
	a_Y = m_Y;
}
