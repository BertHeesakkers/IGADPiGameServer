#include "Framework/AssertMessage.h"
#include "Framework/CommandLine.h"
#include "Framework/CommandLineParser.h"
#include "Framework/HashedString.h"
#include "Framework/StringFunctions.h"
#include "Network/CheckStartupResult.h"
#include "Network/CheckConnectionAttemptResult.h" 
#include "Network/MessageHelpers.h"
#include "Network/RakNetIncludes.h"

#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

namespace
{
	const std::string g_RPCGetLobbyData("get_lobby_data");
	const std::string g_RPCGetGameData("get_game_data");
	const std::string g_RPCKillGame("kill_game");
	const std::string g_RPCKillAllGames("kill_all_games");
	const std::string g_RPCGetPlayerData("get_player_data");
	const std::string g_RPCKillPlayer("kill_player");
	const std::string g_RPCKillAllPlayers("kill_all_players");
	const std::string g_RPCKillServer("kill_server");
}

int main(int a_ArgC, const char * a_ArgV[])
{
	CommandLineParser commandLineParser;
	commandLineParser.AddParameter(ECommandLineItemType_Choice, g_RPCGetLobbyData);
	commandLineParser.AddParameter(ECommandLineItemType_Choice, g_RPCGetGameData);
	commandLineParser.AddParameter(ECommandLineItemType_Optional, g_RPCKillGame);
	commandLineParser.AddParameter(ECommandLineItemType_Choice, g_RPCKillAllGames);
	commandLineParser.AddParameter(ECommandLineItemType_Choice, g_RPCGetPlayerData);
	commandLineParser.AddParameter(ECommandLineItemType_Optional, g_RPCKillPlayer);
	commandLineParser.AddParameter(ECommandLineItemType_Choice, g_RPCKillAllPlayers);
	commandLineParser.AddParameter(ECommandLineItemType_Choice, g_RPCKillServer);
	const CommandLine &commandLine = commandLineParser.Parse(a_ArgC, a_ArgV);

	RakNet::RakPeerInterface *m_PeerInterface = RakNet::RakPeerInterface::GetInstance();
	RakNet::RPC4 rpcInterface;
	m_PeerInterface->AttachPlugin(&rpcInterface);

	const unsigned int maxConnections = 1;
	const unsigned int socketDescriptorCount = 1;
	RakNet::SocketDescriptor socketDescriptor;
	RakNet::StartupResult startUpResult = m_PeerInterface->Startup(maxConnections, &socketDescriptor, socketDescriptorCount);
	CheckStartupResult(startUpResult);

	const std::string serverAddress = "127.0.0.1";
	const uint32_t serverPort = 60000;
	RakNet::ConnectionAttemptResult connectionAttemptResult = m_PeerInterface->Connect(serverAddress.c_str(), serverPort, nullptr, 0);
	CheckConnectionAttemptResult(connectionAttemptResult);

	MessageResult result;
	do
	{
		result = WaitForMessage(*m_PeerInterface, ID_CONNECTION_REQUEST_ACCEPTED, 5000);
		AssertMessage(nullptr != result.m_Packet, "Connection request timed out!");
		if (nullptr == result.m_Packet)
		{
			std::cout << "Unable to connect to server." << std::endl;
			return -1;
		}
	} while (nullptr == result.m_Packet);
	RakNet::SystemAddress serverSystemAddress = result.m_Packet->systemAddress;

	RakNet::BitStream parameters;
	std::string commandID;
	if (commandLine.HasParameter(g_RPCGetLobbyData))
	{
		commandID = "RPCGetLobbyData";
	}
	else if (commandLine.HasParameter(g_RPCGetGameData))
	{
		commandID = "RPCGetGameData";
	}
	else if (commandLine.HasParameter(g_RPCKillGame))
	{
		const uint32_t gameID = commandLine.GetParameter<uint32_t>(g_RPCKillGame);
		parameters.Write(gameID);
		commandID = "RPCKillGame";
	}
	else if (commandLine.HasParameter(g_RPCKillAllGames))
	{
		commandID = "RPCKillAllGames";
	}
	else if (commandLine.HasParameter(g_RPCGetPlayerData))
	{
		commandID = "RPCGetPlayerData";
	}
	else if (commandLine.HasParameter(g_RPCKillPlayer))
	{
		const uint32_t playerID = commandLine.GetParameter<uint32_t>(g_RPCKillPlayer);
		parameters.Write(playerID);
		commandID = "RPCKillPlayer";
	}
	else if (commandLine.HasParameter(g_RPCKillAllPlayers))
	{
		commandID = "RPCKillAllPlayers";
	}
	else if (commandLine.HasParameter(g_RPCKillServer))
	{
		commandID = "RPCKillServer";
	}
	else
	{
		std::cout << "Invalid parameter passed." << std::endl;
		return -2;
	}

	RakNet::BitStream returnData;
	rpcInterface.CallBlocking(commandID.c_str(), &parameters, HIGH_PRIORITY, RELIABLE_ORDERED, 0, serverSystemAddress, &returnData);

	RakNet::RakString data;
	const bool returnDataReceived = returnData.ReadCompressed(data);
	if (returnDataReceived)
	{
		std::vector<std::string> tokens = Tokenize(std::string(data.C_String()), std::string(";"));
		for (auto const &item : tokens)
		{
			std::cout << item << std::endl;
		}
	}

	return 0;
}
