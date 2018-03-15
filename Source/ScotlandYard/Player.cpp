#include "Player.h"

#include "Framework/AssertMessage.h"
#include "Spy.h"

Player::Player()
{
}

Player::~Player()
{
}

uint32_t Player::GetPosition() const
{
	return m_Position;
}

EPlayer Player::GetPlayer() const
{
	return m_Player;
}

bool Player::IsSpy() const
{
	return false;
}

bool Player::HasTokens(ETravelOption a_TravelOption) const
{
	bool result = false;
	switch (a_TravelOption)
	{
	case ETravelOption_Taxi:
	{
		result = (0 != m_TaxiTokens);
		break;
	}
	case ETravelOption_Bus:
	{
		result = (0 != m_BusTokens);
		break;
	}
	case ETravelOption_Underground:
	{
		result = (0 != m_UndergroundTokens);
		break;
	}
	case ETravelOption_Ferry:
	{
		if (IsSpy())
		{
			const Spy *spy = static_cast<const Spy*>(this);
			result = 0 != (spy->m_BlackTokens);
		}
		else
		{
			AssertMessage("Invalid travel option for this player encountered!");
		}
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
	return result;
}

uint32_t Player::GetTokens(ETravelOption a_TravelOption) const
{
	uint32_t result = 0;
	switch (a_TravelOption)
	{
	case ETravelOption_Taxi:
		{
			result = m_TaxiTokens;
			break;
		}
	case ETravelOption_Bus:
		{
			result = m_BusTokens;
			break;
		}
	case ETravelOption_Underground:
		{
			result = m_UndergroundTokens;
			break;
		}
	case ETravelOption_Black:
		{
			if (IsSpy())
			{
				const Spy *spy = static_cast<const Spy*>(this);
				result = spy->m_BlackTokens;
			}
			else
			{
				AssertMessage("Invalid travel option for this player encountered!");
			}
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
	return result;
}

void Player::DropToken(ETravelOption a_TravelOption)
{
	AssertMessage(HasTokens(a_TravelOption), "Player does not have travel tokens left of the requested type!");
	switch (a_TravelOption)
	{
		case ETravelOption_Taxi:
		{
			m_TaxiTokens--;
			break;
		}
		case ETravelOption_Bus:
		{
			m_BusTokens--;
			break;
		}
		case ETravelOption_Underground:
		{
			m_UndergroundTokens--;
			break;
		}
		case ETravelOption_Black:	// to be used for ETravelOption_Ferry as well
		{
			if (IsSpy())
			{
				Spy *spy = static_cast<Spy*>(this);
				spy->m_BlackTokens--;
			}
			else
			{
				AssertMessage("Invalid travel option for this player encountered!");
			}
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

void Player::UpdateTravelLog(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption)
{
	m_TravelLog.AddLogItem(a_Start, a_End, a_TravelOption);
}

const TravelLog& Player::GetTravelLog() const
{
	return m_TravelLog;
}
