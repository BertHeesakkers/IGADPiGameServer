#pragma once

#include "ETravelOption.h"

#include <stdint.h>
#include <vector>

class TravelLog
{
public:
	TravelLog();
	~TravelLog();

	void AddLogItem(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption);
	uint32_t GetNumItems() const;
	void GetLogItem(uint32_t a_Index, uint32_t &a_Start, uint32_t &a_End, ETravelOption &a_TravelOption) const;

	void Reset();

private:
	struct LogItem
	{
		uint32_t m_Start;
		uint32_t m_End;
		ETravelOption m_TravelOption;
	};

	std::vector<LogItem> m_LogItems;

	friend void ConvertToSpyLog(const TravelLog&, TravelLog&);
};

void ConvertToSpyLog(const TravelLog &a_TravelLog, TravelLog &a_SpyTravelLog);
