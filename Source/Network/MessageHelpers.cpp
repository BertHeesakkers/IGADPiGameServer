#include "MessageHelpers.h"

#include "Network/EMessages.h"
#include "Network/EServerErrors.h"
#include "Network/PacketHelpers.h"
#include "Framework/AssertMessage.h"
#include "Framework/Stopwatch.h"

#include <algorithm>
#include <vector>


namespace
{
	Stopwatch g_Stopwatch;
}

bool SendMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, RakNet::MessageID a_MessageID)
{
	AssertMessage(a_SystemAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS, "SystemAddress is not defined! Unable to send message!");
	RakNet::BitStream bsOut;
	bsOut.Write(a_MessageID);
	const uint32_t result = a_Peer.Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, a_SystemAddress, false);
	AssertMessage(0 != result, "Sending message failed!");
	return result != 0;
}

bool SendMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, RakNet::BitStream &a_Payload, bool a_BroadCast)
{
	AssertMessage(a_SystemAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS, "SystemAddress is not defined! Unable to send message!");
	const uint32_t result = a_Peer.Send(&a_Payload, HIGH_PRIORITY, RELIABLE_ORDERED, 0, a_SystemAddress, a_BroadCast);	
	AssertMessage(0 != result, "Sending message failed!");
	return result != 0;
}

bool SendServerErrorMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress, EServerError a_ServerError)
{
	AssertMessage(a_SystemAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS, "SystemAddress is not defined! Unable to send message!");
	RakNet::BitStream bsOut;
	bsOut.Write(static_cast<RakNet::MessageID>(EMessage_RecvServerError));
	bsOut.Write(a_ServerError);
	const uint32_t result = a_Peer.Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, a_SystemAddress, false);
	AssertMessage(0 != result, "Sending message failed!");
	return result != 0;
}

bool SendAcknowledgementMessage(RakNet::RakPeerInterface &a_Peer, const RakNet::SystemAddress &a_SystemAddress)
{
	AssertMessage(a_SystemAddress != RakNet::UNASSIGNED_SYSTEM_ADDRESS, "SystemAddress is not defined! Unable to send message!");
	RakNet::BitStream bsOut;
	bsOut.Write(static_cast<RakNet::MessageID>(EMessage_RecvAcknowledgement));
	const uint32_t result = a_Peer.Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, a_SystemAddress, false);
	AssertMessage(0 != result, "Sending message failed!");
	return result != 0;
}

MessageResult WaitForMessage(RakNet::RakPeerInterface &a_Peer, EMessages a_MessageID, uint64_t a_TimeOut /* = 5000 */)
{
	return WaitForMessage(a_Peer, static_cast<DefaultMessageIDTypes>(a_MessageID), a_TimeOut);
}

#if defined(__linux__)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wenum-compare"

#endif

MessageResult WaitForMessage(RakNet::RakPeerInterface &a_Peer, DefaultMessageIDTypes a_MessageID, uint64_t a_TimeOut /* = 5000 */)
{
	//bool timeOut = false;

	g_Stopwatch.Start();

	MessageResult result;
	result.m_Packet = nullptr;
	do
	{
		for (result.m_Packet = a_Peer.Receive(); 0 != result.m_Packet; a_Peer.DeallocatePacket(result.m_Packet), result.m_Packet = a_Peer.Receive())
		{
			DefaultMessageIDTypes messageID = static_cast<DefaultMessageIDTypes>(result.m_Packet->data[0]);
			if (messageID == a_MessageID)
			{
				result.m_Message = messageID;
				result.m_MessageReceived = true;
				break;
			}
			else if (messageID == EMessage_RecvServerError)
			{
				result.m_ErrorReceived = true;
				result.m_ServerError = EServerError_GeneralError;
				RetrieveMessageData(*result.m_Packet, result.m_ServerError, EMessage_RecvServerError);
				break;
			}
		}

		if (a_TimeOut > 0 && g_Stopwatch.GetTimeSinceStart() > a_TimeOut)
		{
			result.m_TimeoutOccurred = true;
		}
	} while (!result.m_TimeoutOccurred && !result.m_MessageReceived && !result.m_ErrorReceived);

	g_Stopwatch.Stop();

	return result;
}

MessageResult WaitForMessages(RakNet::RakPeerInterface &a_Peer, std::initializer_list<EMessages> a_MessageIDs, uint64_t a_TimeOut /* = 5000 */)
{
	std::vector<DefaultMessageIDTypes> messageIDs;
	for (std::initializer_list<EMessages>::iterator pos = a_MessageIDs.begin(); pos != a_MessageIDs.end(); ++pos)
	{
		EMessages messageID = *pos;
		messageIDs.push_back(static_cast<DefaultMessageIDTypes>(messageID));
	}

	g_Stopwatch.Start();

	MessageResult result;
	result.m_Packet = nullptr;
	do
	{
		for (result.m_Packet = a_Peer.Receive(); 0 != result.m_Packet; a_Peer.DeallocatePacket(result.m_Packet), result.m_Packet = a_Peer.Receive())
		{
			DefaultMessageIDTypes messageID = static_cast<DefaultMessageIDTypes>(result.m_Packet->data[0]);
			if (std::find(messageIDs.begin(), messageIDs.end(), messageID) != messageIDs.end())
			{
				result.m_Message = messageID;
				result.m_MessageReceived = true;
				break;
			}
			else if (messageID == EMessage_RecvServerError)
			{
				result.m_ErrorReceived = true;
				result.m_ServerError = EServerError_GeneralError;
				RetrieveMessageData(*result.m_Packet, result.m_ServerError, EMessage_RecvServerError);
				break;
			}
		}

		if (a_TimeOut > 0 && g_Stopwatch.GetTimeSinceStart() > a_TimeOut)
		{
			result.m_TimeoutOccurred = true;
		}
	} while (!result.m_TimeoutOccurred && !result.m_MessageReceived && !result.m_ErrorReceived);

	g_Stopwatch.Stop();

	return result;
}

#if defined(__linux__)

#pragma GCC diagnostic pop

#endif
