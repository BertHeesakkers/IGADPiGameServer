#pragma once

#include "RakNetIncludes.h"

class ILogger;
void CheckStartupResult(RakNet::StartupResult a_Result);
void CheckStartupResult(RakNet::StartupResult a_Result, ILogger &a_Logger);
