#include "OutputLevel.h"

#include "Framework/AssertMessage.h"
#include "Level.h"

namespace
{
	void OutputElement(EElement a_Element, std::ostream &a_Stream)
	{
		switch (a_Element)
		{
		case EElement_Wall:
			{
				a_Stream << "#";
			}
			break;
		case EElement_Player:
			{
				a_Stream << "@";
			}
			break;
		case EElement_PlayerOnGoalSquare:
			{
				a_Stream << "+";
			}
			break;
		case EElement_Box:
			{
				a_Stream << "$";
			}
			break;
		case EElement_BoxOnGoalSquare:
			{
				a_Stream << "*";
			}
			break;
		case EElement_GoalSquare:
			{
				a_Stream << ".";
			}
			break;
		case EElement_Floor:
			{
				a_Stream << " ";
			}
			break;
		case EElement_Count:
		case EElement_Invalid:
		default:
			{
				AssertMessage("Unknown or invalid element in encountered in level!");
			}
		break;
		}
	}
}

void OutputLevel(const Level &a_Level, std::ostream &a_Stream)
{
	for (uint32_t y = 0; y < a_Level.m_Height; ++y)
	{
		for (uint32_t x = 0; x < a_Level.m_Width; ++x)
		{
			const EElement element = a_Level.m_Data[y * a_Level.m_Width + x];
			OutputElement(element, a_Stream);
		}
		a_Stream << std::endl;
	}
}
