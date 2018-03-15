#include "PacketHelpers.h"

#include "EServerErrors.h"

// template<>
// void ReadBitStream(RakNet::BitStream &a_BitStream, GameJoinedMessage &a_Buffer)
// {
// 	a_BitStream.Read(a_Buffer.m_ClientID);
// 	a_BitStream.Read(a_Buffer.m_GameID);
// }
// 
template<>
void ReadBitStream(RakNet::BitStream &a_BitStream, EServerError &a_Buffer)
{
	(void)a_Buffer;
	(void)a_BitStream;
}
// 
// template<>
// void ReadBitStream(RakNet::BitStream &a_BitStream, RetrieveGameIDMessage &a_Buffer)
// {
// 	a_BitStream.Read(a_Buffer.m_ClientID);
// }
// 
// template<>
// void ReadBitStream(RakNet::BitStream &a_BitStream, JoinGameMessage &a_Buffer)
// {
// 	a_BitStream.Read(a_Buffer.m_ClientID);
// 	a_BitStream.Read(a_Buffer.m_CreateAIOpponent);
// }
// 
// ClientID RetrieveClientID(RakNet::Packet &a_Packet)
// {
// 	ClientID clientID = InvalidClientID();
// 	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
// 	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
// 	bitStream.Read(clientID);
// 	return clientID;
// }
// 
// EMessages RetrieveMessageID(RakNet::Packet &a_Package)
// {
// 	EMessages messageID = EMessages_Invalid;
// 	messageID = static_cast<EMessages>(a_Package.data[0]);
// 	return messageID;
// }
