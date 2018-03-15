#include "Spy.h"

#include "Framework/AssertMessage.h"

Spy::Spy()
{
}

Spy::~Spy()
{
}

bool Spy::IsSpy() const
{
	return true;
}

void Spy::AddToken(ETravelOption a_TravelOption)
{
	switch (a_TravelOption)
	{
	case ETravelOption_Taxi:
		{
			m_TaxiTokens++;
			break;
		}
	case ETravelOption_Bus:
		{
			m_BusTokens++;
			break;
		}
	case ETravelOption_Underground:
		{
			m_UndergroundTokens++;
			break;
		}
	case ETravelOption_Ferry:
		{
			m_BlackTokens++;
			break;
		}
	case ETravelOption_Count:
	case ETravelOption_Invalid:
	default:
		{
			AssertMessage("Invalid travel option encountered!");
			break;
		}
	}
}

const TravelLog& Spy::GetTravelLog() const
{
	return m_TravelLog;
}
