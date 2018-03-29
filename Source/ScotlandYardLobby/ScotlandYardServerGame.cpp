#include "ScotlandYardServerGame.h"

#include "EMessages.h"
#include "EMessagesExt.h"
#include "Framework/AssertMessage.h"
#include "Framework/FileHelpers.h"
#include "Framework/Logging/ILogger.h"
#include "Network/EMessages.h"
#include "Network/MessageHelpers.h"
#include "ScotlandYard/Player.h"
#include "ScotlandYard/ScotlandYardGame.h"
#include "ScotlandYard/Spy.h"

namespace
{
	const std::string g_GameName = "ScotlandYard";
	std::string g_Map = "";

	void RetrievTravelData(RakNet::Packet &a_Packet, uint32_t &a_Destination, ETravelOption &a_TravelOption)
	{
		RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
		bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
		bitStream.IgnoreBytes(sizeof(ClientID));
		bitStream.IgnoreBytes(sizeof(GameID));
		bitStream.Read(a_Destination);
		short travelOption;
		bitStream.Read(travelOption);
		a_TravelOption = static_cast<ETravelOption>(travelOption);
	}

	void WriteTravelLog(const TravelLog &a_TravelLog, RakNet::BitStream &a_Payload)
	{
		uint32_t start = 0;
		uint32_t end = 0;
		ETravelOption travelOption = ETravelOption_Invalid;
		const uint32_t numLogItems = a_TravelLog.GetNumItems();
		a_Payload.Write(numLogItems);
		for (uint32_t index = 0; index < numLogItems; ++index)
		{
			a_TravelLog.GetLogItem(index, start, end, travelOption);
			a_Payload.Write(end);
			a_Payload.Write(static_cast<short>(travelOption));
		}
	}
}

ScotlandYardServerGame::ScotlandYardServerGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface, ILogger &a_Logger)
	: BaseServerGame(a_GameID, a_PeerInterface, a_Logger)
{
	m_Game = new ScotlandYardGame();
	g_Map = ReadFileToString(ScotlandYardGame::GetMapFilename());
}

ScotlandYardServerGame::~ScotlandYardServerGame()
{
	delete m_Game;
}

uint32_t ScotlandYardServerGame::GetNumPlayersPerGame()
{
	return ScotlandYardGame::GetNumPlayersPerGame();
}

ClientID ScotlandYardServerGame::WhoseTurnIsIt() const
{
	return GetClient(m_Game->WhoseTurnIsIt());
}

bool ScotlandYardServerGame::HandleGameMessage(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	bool messageHandled = false;
	const unsigned char messageID = static_cast<const unsigned char>(a_Packet.data[0]);
	switch (messageID)
	{
	case EMessage_SendGetMap:
		{
			HandleGetMap(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendGetSpyTravelLog:
		{
			HandleGetSpyTravelLog(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendGetPlayerLocations:
		{
			HandleGetPlayerLocations(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendTravel:
		{
			HandleTravel(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendGetLocation:
		{
			HandleGetLocation(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendGetTravelLog:
		{
			HandleGetTravelLog(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendAmISpy:
		{
			HandleGetAmISpy(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendWhoIsSpy:
		{
			HandleGetAmISpy(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	case EMessage_SendGetRemainingTokens:
		{
			HandleGetRemainingTokens(a_Packet, a_ClientID);
			messageHandled = true;
			break;
		}
	default:
		{
			SendServerErrorMessage(GetPeerInterface(), a_Packet.systemAddress, EServerError_InvalidGameCommand);
			break;
		}
	}
	return messageHandled;
}

std::string ScotlandYardServerGame::GetGameName() const
{
	return g_GameName;
}

void ScotlandYardServerGame::AddPlayer()
{
	const EPlayer newPlayer = static_cast<EPlayer>(m_Game->GetNumPlayers());
	m_Game->AddPlayer(newPlayer);
}

void ScotlandYardServerGame::HandleGetMap(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	ILogger &logger = GetServerLogger();
	if (m_Game->IsActive())
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetMap));
		payload.Write(RakNet::RakString(g_Map.c_str()));
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleGetSpyTravelLog(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const TravelLog& travelLog = m_Game->GetSpyTravelLog();

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetSpyTravelLog));
		WriteTravelLog(travelLog, payload);

		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleGetPlayerLocations(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		//const EPlayer currentPlayer = GetPlayer(a_ClientID);
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetPlayerLocations));

		const ScotlandYardGame &game = *const_cast<ScotlandYardGame*>(m_Game);
		const uint32_t numPlayers = GetNumPlayersPerGame();
		for (uint32_t playerIndex = 0; playerIndex < numPlayers; ++playerIndex)
		{
			EPlayer playerID = static_cast<EPlayer>(playerIndex);
			const Player &player = game.GetPlayer(playerID);
			if (!player.IsSpy())
			{
				payload.Write(playerID);
				payload.Write(GetClient(playerID));
				payload.Write(player.GetPosition());
			}
		}
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleTravel(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer player = GetPlayer(a_ClientID);
		if (player == m_Game->WhoseTurnIsIt())
		{
			uint32_t destination;
			ETravelOption travelOption;
			RetrievTravelData(a_Packet, destination, travelOption);
			ETravelResult result = m_Game->Travel(player, destination, travelOption);
			if (ETravelResult_Success == result)
			{
				BroadcastTurnFinished(a_ClientID);
			}
			RakNet::BitStream payload;
			payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvTravelResult));
			payload.Write(static_cast<short>(result));
			SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
		}
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleGetLocation(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer currentPlayer = GetPlayer(a_ClientID);
		const ScotlandYardGame &game = *const_cast<ScotlandYardGame*>(m_Game);
		const Player &player = game.GetPlayer(currentPlayer);

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetLocation));
		payload.Write(player.GetPosition());
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleGetTravelLog(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer currentPlayer = GetPlayer(a_ClientID);
		const ScotlandYardGame &game = *const_cast<ScotlandYardGame*>(m_Game);
		const Player &player = game.GetPlayer(currentPlayer);
		const TravelLog &travelLog = player.GetTravelLog();

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetTravelLog));
		WriteTravelLog(travelLog, payload);
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleGetAmISpy(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer currentPlayer = GetPlayer(a_ClientID);
		const ScotlandYardGame &game = *const_cast<ScotlandYardGame*>(m_Game);
		const Player &player = game.GetPlayer(currentPlayer);

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvAmISpy));
		payload.Write(player.IsSpy());
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleWhoIsSpy(RakNet::Packet& a_Packet)
{
	if (m_Game->IsActive())
	{
		const ScotlandYardGame &game = *const_cast<ScotlandYardGame*>(m_Game);
		const Spy &spy = game.GetSpy();

		const ClientID clientID = GetClient(spy.GetPlayer());

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvWhoIsSpy));
		payload.Write(clientID);
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

void ScotlandYardServerGame::HandleGetRemainingTokens(RakNet::Packet &a_Packet, ClientID a_ClientID)
{
	if (m_Game->IsActive())
	{
		const EPlayer currentPlayer = GetPlayer(a_ClientID);
		const ScotlandYardGame &game = *const_cast<ScotlandYardGame*>(m_Game);
		const Player &player = game.GetPlayer(currentPlayer);

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGetRemainingTokens));
		payload.Write(static_cast<short>(ETravelOption_Taxi));
		payload.Write(player.GetTokens(ETravelOption_Taxi));
		payload.Write(static_cast<short>(ETravelOption_Bus));
		payload.Write(player.GetTokens(ETravelOption_Bus));
		payload.Write(static_cast<short>(ETravelOption_Underground));
		payload.Write(player.GetTokens(ETravelOption_Underground));
		if (player.IsSpy())
		{
			payload.Write(static_cast<short>(ETravelOption_Black));
			payload.Write(player.GetTokens(ETravelOption_Black));
		}
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, payload);
	}
	else
	{
		SendNetworkMessage(GetPeerInterface(), a_Packet.systemAddress, EMessage_RecvGameNotActive);
	}
}

