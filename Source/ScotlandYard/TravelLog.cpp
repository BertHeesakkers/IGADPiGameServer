#include "TravelLog.h"

#include "Framework/AssertMessage.h"

#include <algorithm>
#include <iterator>

TravelLog::TravelLog()
{
}

TravelLog::~TravelLog()
{
}

void TravelLog::AddLogItem(uint32_t a_Start, uint32_t a_End, ETravelOption a_TravelOption)
{
	LogItem logItem;
	logItem.m_Start = a_Start;
	logItem.m_End = a_End;
	logItem.m_TravelOption = a_TravelOption;
	m_LogItems.push_back(logItem);
}

uint32_t TravelLog::GetNumItems() const
{
	return static_cast<uint32_t>(m_LogItems.size());
}

void TravelLog::GetLogItem(uint32_t a_Index, uint32_t &a_Start, uint32_t &a_End, ETravelOption &a_TravelOption) const
{
	AssertMessage(a_Index < static_cast<uint32_t>(m_LogItems.size()), "Invalid log item requested!");
	const LogItem &logItem = m_LogItems[a_Index];
	a_Start = logItem.m_Start;
	a_End = logItem.m_End;
	a_TravelOption = logItem.m_TravelOption;
}

void TravelLog::Reset()
{
	m_LogItems.clear();
}

void ConvertToSpyLog(const TravelLog &a_TravelLog, TravelLog &a_SpyTravelLog)
{
	const uint32_t turns[] = { 3, 8, 13, 18 };
	for (auto pos = a_TravelLog.m_LogItems.begin(); pos != a_TravelLog.m_LogItems.end(); ++pos)
	{
		const TravelLog::LogItem &logItem = *pos;
		TravelLog::LogItem newItem;
		newItem.m_Start = 0;
		newItem.m_TravelOption = logItem.m_TravelOption;

		const size_t currentTurn = pos - a_TravelLog.m_LogItems.begin();
		const bool emerge = std::any_of(std::begin(turns), std::end(turns), [&](size_t turn) { return turn == currentTurn; });
		newItem.m_End = emerge ? logItem.m_End : 0;

		a_SpyTravelLog.m_LogItems.push_back(newItem);
	}
}
