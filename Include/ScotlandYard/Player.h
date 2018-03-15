#pragma once

#include "Network/EPlayer.h"
#include "ETravelOption.h"
#include "TravelLog.h"

#include <stdint.h>

class Player
{
public:
	Player();
	~Player();

	uint32_t GetPosition() const;
	EPlayer GetPlayer() const;
	virtual bool IsSpy() const;
	bool HasTokens(ETravelOption a_TravelOption) const;
	uint32_t GetTokens(ETravelOption a_TravelOption) const;
	void DropToken(ETravelOption a_TravelOption);
	void UpdateTravelLog(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption);
	virtual const TravelLog& GetTravelLog() const;

protected:
	uint32_t m_TaxiTokens = 10;
	uint32_t m_BusTokens = 8;
	uint32_t m_UndergroundTokens = 4;
	TravelLog m_TravelLog;

private:
	EPlayer m_Player = EPlayer_Invalid;
	uint32_t m_Position = static_cast<uint32_t>(-1);

	friend class ScotlandYardGame;
};
