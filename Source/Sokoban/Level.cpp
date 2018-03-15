#include "Level.h"

#include <cstring>

Level::Level(uint32_t a_Width, uint32_t a_Height)
	: m_Width(a_Width)
	, m_Height(a_Height)
{
	m_Data = new EElement[a_Width * a_Height];
	::memset(m_Data, EElement_Floor, m_Width * m_Height);
}

Level::~Level()
{
	delete [] m_Data;
}
