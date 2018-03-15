#pragma once

class Map;
class Ply;
class TravelLog;

#include <vector>

void ValidateTravelLog(const TravelLog &a_TravelLog, const Map &a_Map);
void ValidatGame(const std::vector<const TravelLog&> &a_TravelLogs, const Map &a_Map, const Ply &a_Ply);
