#pragma once

#include <stdint.h>

class Node
{
public:
	Node(uint32_t a_Index);
	~Node();

	uint32_t GetX() const;
	uint32_t GetY() const;
	void GetCoordinates(uint32_t &a_X, uint32_t &a_Y);

private:
	const uint32_t m_Index;
	uint32_t m_X;
	uint32_t m_Y;

	friend class Map;
	friend class ScotlandYardGame;
};