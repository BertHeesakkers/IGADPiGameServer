#include "ValidateTravelLog.h"

#include "Framework/AssertMessage.h"
#include "Map.h"
#include "Ply.h"
#include "TravelLog.h"

void ValidateTravelLog(const TravelLog &a_TravelLog, const Map &a_Map)
{
	uint32_t source;
	uint32_t destinaton;
	ETravelOption travelOption;

	bool success = true;
	a_TravelLog.GetLogItem(0, source, destinaton, travelOption);
	success = a_Map.CanTravel(source, destinaton, travelOption);
	AssertMessage(success, "Invalid travel log: can't travel between start and end with the specified travel option!");
	uint32_t previousDestination = destinaton;

	const uint32_t numItems = a_TravelLog.GetNumItems();
	for (uint32_t index = 1; index < numItems; ++index)
	{
		success = a_Map.CanTravel(source, destinaton, travelOption);
		AssertMessage(success, "Invalid travel log: can't travel between start and end with the specified travel option!");
		success = previousDestination == source;
		AssertMessage(success, "Invalid travel log: previous destination and current start do not match!");
	}
}

void ValidatGame(const std::vector<const TravelLog*> &a_TravelLogs, const Map &a_Map, const Ply &a_Ply)
{
	for (auto pos = a_TravelLogs.begin(); pos != a_TravelLogs.end(); ++pos)
	{
		ValidateTravelLog(**pos, a_Map);

		// check that position are not being occupied by two players at the same time
	}
}
