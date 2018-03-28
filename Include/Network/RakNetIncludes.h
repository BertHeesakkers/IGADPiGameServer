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
#include "RakNet/RPC4Plugin.h"
/**
* Added by Simon Renger
* Date: 27-03-2018
* Description:
* Added it because it gives access to the RakPeer. This gives you access to f.g. RakNet::RakPeer::NumberOfConnections
**/
#include "RakNet/RakPeer.h"

#if defined(WIN32)
#pragma warning(pop)
#endif