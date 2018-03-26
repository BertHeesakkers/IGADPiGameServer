#pragma once

#include "RakNetIncludes.h"
#include "EMessages.h"
#include "EServerErrors.h"
#include "Network/WriteBitStream.h"

#include <initializer_list>

struct MessageResult
{
	bool m_MessageReceived = false;
	bool m_ErrorReceived = false;
	bool m_TimeoutOccurred = false;
	EServerError m_ServerError = EServerError_NoError;
	DefaultMessageIDTypes m_Message;
	RakNet::Packet *m_Packet = nullptr;
};

bool SendNetworkMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, RakNet::MessageID a_MessageID);
bool SendNetworkMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, RakNet::BitStream &a_Payload, bool a_BroadCast = false);
bool SendServerErrorMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, EServerError a_ServerError);
bool SendAcknowledgementMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress);

template<typename TYPE>
bool SendNetworkMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, EMessages a_MessageID, const TYPE &a_Buffer)
{
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(a_MessageID));
	WriteBitStream(bitStream, a_Buffer);

	const uint32_t result = a_Peer.Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, a_SystemAddress, false);
	return result != 0;
}

MessageResult WaitForMessage(RakNet::RakPeerInterface &a_Peer, EMessages a_MessageID, uint64_t a_TimeOut = 5000);
MessageResult WaitForMessage(RakNet::RakPeerInterface &a_Peer, DefaultMessageIDTypes a_MessageID, uint64_t a_TimeOut = 5000);
MessageResult WaitForMessages(RakNet::RakPeerInterface &a_Peer, std::initializer_list<EMessages> a_MessageIDs, uint64_t a_TimeOut = 5000);
