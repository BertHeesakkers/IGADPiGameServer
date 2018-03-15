#pragma once

#include "ETravelOption.h"
#include "Player.h"

class Spy : public Player
{
public:
	Spy();
	~Spy();

	virtual bool IsSpy() const override;
	void AddToken(ETravelOption a_TravelOption);
	virtual const TravelLog& GetTravelLog() const override;

private:
	uint32_t m_BlackTokens;

	friend class Player;
};