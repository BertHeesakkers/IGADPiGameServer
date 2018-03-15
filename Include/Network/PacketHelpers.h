#pragma once

#include "Network/EMessages.h"

#include "RakNetIncludes.h"
// #include "ClientID.h"
// #include "MessageTypes.h"

// EMessages RetrieveMessageID(RakNet::Packet &a_Packet);
// ClientID RetrieveClientID(RakNet::Packet &a_Packet);

template<typename TYPE>
void ReadBitStream(RakNet::BitStream &a_BitStream, TYPE &a_Buffer);

template<typename TYPE>
bool RetrieveMessageData(RakNet::Packet &a_Packet, TYPE &a_Buffer, EMessages a_MessageID)
{
	bool success = false;

	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	const DefaultMessageIDTypes messageID = static_cast<DefaultMessageIDTypes>(a_Packet.data[0]);
	if (messageID == static_cast<DefaultMessageIDTypes>(a_MessageID))
	{
		bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
		ReadBitStream(bitStream, a_Buffer);
		success = true;
	}

	return success;
}
