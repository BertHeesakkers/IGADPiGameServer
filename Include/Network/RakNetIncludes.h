#pragma once

#if defined(WIN32)
#pragma warning(push)  
#pragma warning(disable : 4005)
#pragma warning(disable : 4477)
#pragma warning(disable : 4996)
#endif

#include "RakNet/BitStream.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/RakPeerInterface.h"
#include "RakNet/PacketLogger.h"

#if defined(WIN32)
#pragma warning(pop)
#endif