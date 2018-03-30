#include "Server.h"

#include "Framework/AssertMessage.h"
#include "Framework/DateTimeHelpers.h"
#include "Framework/FileHelpers.h"
#include "Framework/Logging/FileLogger.h"
#include "Framework/Logging/ILogger.h"
#include "Framework/Sleep.h"
#include "Framework/StringFunctions.h"
#include "Network/CheckStartupResult.h"
#include "Network/ClientID.h"
#include "Network/EMessages.h"
#include "Network/EServerErrors.h"
#include "Network/GameID.h"
#include "Network/GameHelpers.h"
#include "Network/GenerateClientID.h"
#include "Network/GenerateGameID.h"
#include "Network/Helpers/Translate.h"
#include "Network/MessageHelpers.h"
#include "GameData.h"
#include "Helpers/LobbyHelpers.h"
#include "Helpers/ParseServerParameters.h"
#include "Helpers/UserDataHelpers.h"
#include "ILobby.h"
#include "IServerGame.h"
#include "ServerParameters.h"
#include "UserData.h"
#include "Utility/ReadUserData.h"
#include "Utility/WriteUserData.h"

namespace
{
	const std::string g_ParameterFile = "data/server_parameters.json";
	const std::string g_UserDataFilename = "data/students_2017.json";
	const std::string g_ServerHelpFile = "data/server_help.txt";
	Server *g_Server = nullptr;

	bool IsEmptyPass(const HashedString &a_Passhash)
	{
		return (HashedString::Empty == a_Passhash);
	}

	bool IsEmptyString(const std::string &a_String)
	{
		return (a_String.empty() || a_String[0] == '\0');	// <- odd string behaviour
	}

	void ExtractMessageInfo(RakNet::Packet &a_Packet, ClientID &a_ClientID, GameID &a_GameID)
	{
		RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
		bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
		bitStream.Read(a_ClientID);
		bitStream.Read(a_GameID);
	}

	void HandleSuccessfulLogin(RakNet::RakPeerInterface &a_PeerInterface, RakNet::Packet &a_Packet, UserData &a_UserData, ILogger &a_Logger, bool a_SendMessages = true)
	{
		a_UserData.m_LoggedIn = true;
		a_UserData.m_ClientID = GenerateClientID();
		a_UserData.m_SystemAddress = a_Packet.systemAddress;
		if (a_SendMessages)
		{
			RakNet::BitStream payload;
			payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvLoginSuccess));
			payload.Write(a_UserData.m_ClientID);
			SendNetworkMessage(a_PeerInterface, a_UserData.m_SystemAddress, payload);
		}
	}

	void RPCGetLobbyData(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		std::vector<std::string> lobbyData;
		std::string result = "No lobby data found.";
		if (g_Server->GetLobbyData(lobbyData))
		{
			result = "";
			for (auto const &data : lobbyData)
			{
				result += data;
				result += ";";
			}
		}
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCGetGameData(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		std::vector<std::string> gameData;
		std::string result = "No game data found.";
		if (g_Server->GetGameData(gameData))
		{
			result = "";
			for (auto const &data : gameData)
			{
				result += data;
				result += ";";
			}
		}
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCKillGame(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		uint32_t gameID = InvalidGameID();
		a_Incoming->Read(gameID);
		const std::string result = "RPCKillGame (" + std::to_string(gameID) + ") not implemented!";
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCKillAllGames(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		const std::string result = "RPCKillAllGames not implemented!";
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCGetPlayerData(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		std::vector<std::string> playerData;
		std::string result = "No player data found.";
		if (g_Server->GetPlayerData(playerData))
		{
			result = "";
			for (auto const &data : playerData)
			{
				result += data;
				result += ";";
			}
		}
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCKillPlayer(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		uint32_t clientID = InvalidClientID();
		a_Incoming->Read(clientID);
		const std::string result = "RPCKillPlayer (" + std::to_string(clientID) + ") not implemented!";
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCKillAllPlayers(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		const std::string result = "RPCKillAllPlayers not implemented!";
		a_OutGoing->WriteCompressed(RakNet::RakString(result.c_str()));
	}

	void RPCKillServer(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		g_Server->KillServer();
		a_OutGoing->WriteCompressed(RakNet::RakString("Server killed."));
	}

	void RPCPurgeLogFile(RakNet::BitStream *a_Incoming, RakNet::BitStream *a_OutGoing, RakNet::Packet *a_Packet)
	{
		AssertMessage(nullptr != g_Server, "Attempt to use an invalid server!");
		std::string logFilename;
		g_Server->PurgeLogFile(logFilename);
		a_OutGoing->WriteCompressed(RakNet::RakString(logFilename.c_str()));
	}
}

Server::Server(ILogger &a_Logger, bool a_UsePacketLogger /* = false */)
	: m_Logger(a_Logger)
	, m_UsePacketLogger(a_UsePacketLogger)
{
}

Server::~Server()
{
	RakNet::RakPeerInterface::DestroyInstance(m_PeerInterface);
	m_PeerInterface = nullptr;
	g_Server = nullptr;
}

void Server::Start(ServerParameters &a_ServerParameters)
{
	g_Server = this;

	m_PeerInterface = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor socketDescriptor(a_ServerParameters.m_ServerPort, 0);
	RakNet::StartupResult startupResult = m_PeerInterface->Startup(a_ServerParameters.m_MaxClients, &socketDescriptor, 1);
	CheckStartupResult(startupResult, m_Logger);
	m_Logger.WriteLine(StringFormatter::Format("Server socket [%d] open for receive.", a_ServerParameters.m_ServerPort).c_str());
	m_PeerInterface->SetMaximumIncomingConnections(a_ServerParameters.m_MaxClients);

	if (m_UsePacketLogger)
	{
		m_PacketLogger = new RakNet::PacketLogger();
		m_PeerInterface->AttachPlugin(m_PacketLogger);
	}

	for (auto pos = a_ServerParameters.m_Lobbies.begin(); pos != a_ServerParameters.m_Lobbies.end(); ++pos)
	{
		EGame game = *pos;
		AddLobby(game);
	}

	const bool setupRPCFunctions = true;
	if (setupRPCFunctions)
	{
		m_RPCInterface = new RakNet::RPC4();
		m_PeerInterface->AttachPlugin(m_RPCInterface);

		m_RPCInterface->RegisterBlockingFunction("RPCGetLobbyData", RPCGetLobbyData);
		m_RPCInterface->RegisterBlockingFunction("RPCGetGameData", RPCGetGameData);
		m_RPCInterface->RegisterBlockingFunction("RPCKillGame", RPCKillGame);
		m_RPCInterface->RegisterBlockingFunction("RPCKillAllGames", RPCKillAllGames);
		m_RPCInterface->RegisterBlockingFunction("RPCGetPlayerData", RPCGetPlayerData);
		m_RPCInterface->RegisterBlockingFunction("RPCKillPlayer", RPCKillPlayer);
		m_RPCInterface->RegisterBlockingFunction("RPCKillAllPlayers", RPCKillAllPlayers);
		m_RPCInterface->RegisterBlockingFunction("RPCKillServer", RPCKillServer);
	}

	ReadUserData(g_UserDataFilename, m_UserData);
}

void Server::Run()
{
	m_ServerRunning = true;

	RakNet::Packet *packet = nullptr;
	while (m_ServerRunning)
	{
		if (m_UpdateUserData)
		{
			UpdateUserData();
		}
		for (packet = m_PeerInterface->Receive(); nullptr != packet; m_PeerInterface->DeallocatePacket(packet), packet = m_PeerInterface->Receive())
		{
			const DefaultMessageIDTypes messageID = static_cast<DefaultMessageIDTypes>(packet->data[0]);
			switch (messageID)
			{
			case ID_CONNECTION_LOST:
				{
					m_Logger.WriteLine("Lost connection");
					HandleLostConnection(*packet);
					break;
				}
			case ID_NEW_INCOMING_CONNECTION:
				{
					m_Logger.WriteLine("New incoming connection.");
					break;
				}
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				{
					m_Logger.WriteLine("New remote incoming connection.");
					break;
				}
			case ID_DISCONNECTION_NOTIFICATION:
				{
					m_Logger.WriteLine("Client disconnected from server.");
					break;
				}
			case EMessage_SendServerHelp:
				{
					HandleServerHelp(packet->systemAddress);
					break;
				}
			case EMessage_SendGameHelp:
				{
					RakNet::BitStream bitStream(packet->data, packet->length, false);
					bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
					EGame game;
					bitStream.Read(game);
					HandleGameHelp(packet->systemAddress, game);
					break;
				}
			case EMessage_SendLogin:
				{
					RakNet::BitStream bitStream(packet->data, packet->length, false);
					bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
					RakNet::RakString login;
					RakNet::RakString passhash;
					bitStream.Read(login);
					bitStream.Read(passhash);

					HandleLogin(*packet, login.C_String(), passhash.C_String());
					break;
				}
			case EMessage_SendJoinGame:
				{
					RakNet::BitStream bitStream(packet->data, packet->length, false);
					bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
					ClientID clientID = InvalidClientID();
					EGame game = EGame_Invalid;
					bitStream.Read(clientID);
					bitStream.Read(game);

					UserData *userData = FindUserData(m_UserData, clientID);
					if (nullptr == userData)
					{
						SendServerErrorMessage(*m_PeerInterface, packet->systemAddress, EServerError_UserDataUnavailable);
						m_Logger.WriteLine(StringFormatter::Format("Unable to find user data for [%d]!", clientID).c_str());
					}
					else
					{
						HandleJoinGame(game, *userData);
					}
					break;
				}
			case EMessage_SendDisconnect:
				{
					SendNetworkMessage(*m_PeerInterface, packet->systemAddress, EMessage_RecvDisconnected);
					break;
				}
			case EMessage_SendWhoseTurnIsIt:
				{
					HandleDetermineCurrentPlayer(*packet);
					break;
				}
			case EMessage_SendLobbyData:
				{
					HandleSendLobbyData(*packet);
					break;
				}
			default:
				{
					if (!HandleGameMessage(*packet))
					{
						m_Logger.WriteLine("Unknown message ID encountered!");
					}
					break;
				}
			}
		}

		Sleep::MilliSeconds(100);
	}

	if (m_UpdateUserData)
	{
		UpdateUserData();
	}
}

bool Server::IsRunning() const
{
	return m_ServerRunning;
}

void Server::Stop(bool a_SendMessages /* = true */)
{
	m_ServerRunning = false;

	for (auto itLobby = m_Lobbies.begin(); itLobby != m_Lobbies.end(); )
	{
		ILobby &lobby = **itLobby;
		std::vector<UserData*> players = lobby.GetWaitQueue();
// 		std::vector<IServerGame*> games = lobby.GetGames();
// 		for (auto itGame = games.begin(); itGame != games.end(); ++itGame)
// 		{
// 			IServerGame &game = **itGame;
// 			// add players to wait queue
// 		}

		for (auto itPlayer = players.begin(); itPlayer != players.end();)
		{
			UserData &userData = **itPlayer;
			if (a_SendMessages)
			{
				m_PeerInterface->CloseConnection(userData.m_SystemAddress, true, 0, LOW_PRIORITY);
			}
			delete &userData;
			players.erase(itPlayer);
		}
		delete &lobby;
		itLobby = m_Lobbies.erase(itLobby);
	}
}

bool Server::GetLobbyData(std::vector<std::string> &a_Data) const
{
	a_Data.clear();
	return !a_Data.empty();
}

bool Server::GetGameData(std::vector<std::string> &a_Data) const
{
	return false;
}

bool Server::KillGame(const GameID &a_ID)
{
	(void)a_ID;
	return false;
}

bool Server::KillAllGames()
{
	return false;
}

bool Server::GetPlayerData(std::vector<std::string> &a_Data) const
{
	a_Data.clear();
	for (auto posLobby = m_Lobbies.begin(); posLobby != m_Lobbies.end(); ++posLobby)
	{
		const ILobby &lobby = **posLobby;
		a_Data.push_back("LOBBY: " + Translate(lobby.GetGameType()));

		const std::vector<IServerGame*> &games = lobby.GetGames();
		a_Data.push_back("# OF GAMES: " + std::to_string(static_cast<int>(games.size())));

		for (auto posGame = games.begin(); posGame != games.end(); ++posGame)
		{
			const IServerGame &game = **posGame;
			a_Data.push_back("GAME ID: " + std::to_string(static_cast<unsigned int>(game.GetGameID())));

			const UserDataVector &players = game.GetPlayers();
			for (auto userPos = players.begin(); userPos != players.end(); ++userPos)
			{
				UserData &user = **userPos;
				a_Data.push_back("PLAYER NAME: " + user.m_Name);
				a_Data.push_back("PLAYER ID: " + user.m_ID);
				a_Data.push_back("CLIENT ID: " + std::to_string(static_cast<unsigned int>(user.m_ClientID)));
			}
		}
	}
	return !a_Data.empty();
}

bool Server::KillPlayer(const ClientID &a_ID)
{
	(void)a_ID;
	return false;
}

bool Server::KillAllPlayers()
{
	return false;
}

bool Server::KillServer()
{
	m_Logger.WriteLine("Killing server");
	Stop(true);
	return true;
}

bool Server::PurgeLogFile(std::string &a_LogFilename)
{
	bool success = false;
	FileLogger *fileLogger = dynamic_cast<FileLogger*>(&m_Logger);
	if (nullptr != fileLogger)
	{
		const std::string dateTimeString = GetDateTimeFileString();
		fileLogger->SaveLogFile(std::string("serverlog_") + dateTimeString, true);
		success = true;
	}
	return success;
}

void Server::UpdateUserData()
{
	WriteUserData(g_UserDataFilename, m_UserData);
	m_UpdateUserData = false;
}
void Server::HandleLostConnection(RakNet::Packet &a_Packet)
{
	auto* userData = FindUserData(m_UserData,a_Packet.systemAddress);
	if(userData != nullptr)
	{
		m_Logger.WriteLine("Lost connection with client [%s] with the ID [%i].",a_Packet.systemAddress.ToString(true),userData->m_ClientID);
		RemovePlayer(*userData);
	}
		
}

void Server::HandleLogin(RakNet::Packet &a_Packet, const std::string &a_ID, const HashedString &a_Passhash, bool a_SendMessages /* = true */)
{
#if defined(_DEBUG)
	m_Logger.WriteLine("Client [%s] attempt to login with pass [%s].", a_ID, a_Passhash.AsCString());
#else
	m_Logger.WriteLine("Client [%s] attempt to login with pass [%s].", a_ID, a_Passhash.GetHash());
#endif

	UserData *userData = FindUserData(m_UserData, a_ID);
	if (nullptr != userData)
	{
		if (userData->m_LoggedIn)
		{
			m_Logger.WriteLine("Client [%s] attempted to login twice.", a_ID);
			SendNetworkMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_AlreadyLoggedIn);
		}
		else
		{
			const bool emptyPasshash = IsEmptyPass(userData->m_Passhash);
			if (!emptyPasshash)
			{
				const bool success = ValidateLoginData(*userData, a_ID, a_Passhash);
				if (success)
				{
#if defined(_DEBUG)
					m_Logger.WriteLine("Successful login for client [%s] with pass [%s].", a_ID, a_Passhash.AsCString());
#else
					m_Logger.WriteLine("Successful login for client [%s] with pass [%s].", a_ID, a_Passhash.GetHash());
#endif

					HandleSuccessfulLogin(*m_PeerInterface, a_Packet, *userData, m_Logger, a_SendMessages);
				}
				else
				{
#if defined(_DEBUG)
					m_Logger.WriteLine("Unable to verify login data form client [%s] with pass [%s].", a_ID, a_Passhash.AsCString());
#else
					m_Logger.WriteLine("Unable to verify login data form client [%s] with pass [%s].", a_ID, a_Passhash.GetHash());
#endif
					if (a_SendMessages)
					{
						SendServerErrorMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_InvalidPassword);
					}
				}
			}
			else if (emptyPasshash && !IsEmptyPass(a_Passhash))
			{
#if defined(_DEBUG)
				m_Logger.WriteLine("Successful login for client [%s] with pass [%s].", a_ID, a_Passhash.AsCString());
#else
				m_Logger.WriteLine("Successful login for client [%s] with pass [%s].", a_ID, a_Passhash.GetHash());
#endif
				userData->m_Passhash = a_Passhash;
				HandleSuccessfulLogin(*m_PeerInterface, a_Packet, *userData, m_Logger, a_SendMessages);
				m_UpdateUserData = true;
			}
			else
			{
				m_Logger.WriteLine("Unable to login client [%s]; no passhash provided.", a_ID);
				if (a_SendMessages)
				{
					SendNetworkMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_NoPassword);
				}
			}
		}
	}
	else
	{
		SendServerErrorMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_UserDataUnavailable);
		m_Logger.WriteLine(StringFormatter::Format("[HandleLogin] Unable to find user data for [%s]!", a_ID).c_str());
	}
}

void Server::HandleLogout(const std::string &a_ID, bool a_SendMessages /* = true */)
{
	UserData *userData = FindUserData(m_UserData, a_ID);
	AssertMessage(nullptr != userData, "Unable to find user data for client.");
	if (a_SendMessages)
	{
		m_PeerInterface->CloseConnection(userData->m_SystemAddress, true, 0, LOW_PRIORITY);
	}
	RemovePlayer(*userData);
}

void Server::HandleJoinGame(EGame a_Game, UserData &a_UserData, bool a_SendMessages /* = true */)
{
	ILobby *lobby = FindGameLobby(m_Lobbies, a_Game);
	if (nullptr == lobby)
	{
		m_Logger.WriteLine("No game lobby available for game [%s].", Translate(a_Game));
		if (a_SendMessages)
		{
			SendNetworkMessage(*m_PeerInterface, a_UserData.m_SystemAddress, EServerError_GameLobbyUnavailable);
		}
	}
	else
	{
		lobby->AddToQueue(a_UserData);
		
		if (a_SendMessages)
		{
				HandleWaitingFromPlayer(*lobby);
		}

		if (lobby->CanStartNewGame())
		{
			const GameID newGameID = GenerateGameID();
			lobby->AddNewGame(newGameID, *m_PeerInterface);			
			if (a_SendMessages)
			{
				std::vector<UserData*> players;
				FindUserData(m_UserData, newGameID, players);
				for (auto pos = players.begin(); pos != players.end(); ++pos)
				{
					const UserData &userData = **pos;
					RakNet::BitStream payload;
					payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvGameJoined));
					payload.Write(userData.m_ClientID);
					payload.Write(userData.m_GameID);
					SendNetworkMessage(*m_PeerInterface, userData.m_SystemAddress, payload);
					lobby->RemoveFromQueue(userData.m_ClientID);
				}
			}
		}
	}
}

void Server::HandleWaitingFromPlayer(ILobby &a_Lobby)
{
	std::vector<UserData*> players = a_Lobby.GetWaitQueue();
	const uint32_t requiredAmount = a_Lobby.GetNumPlayersPerGame();
	const uint32_t numberOfPlayers = static_cast<uint32_t>(players.size());

	RakNet::BitStream payload;
	payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvWaitingForPlayers));
	payload.Write(requiredAmount);
	payload.Write(numberOfPlayers);

	for (auto pos = players.begin(); pos != players.end(); ++pos)
	{
		const UserData &userData = **pos;
		payload.Write(RakNet::RakString(userData.m_Name.c_str()));
		payload.Write(static_cast<uint32_t>(userData.m_ClientID));
	}

	for (auto pos = players.begin(); pos != players.end(); ++pos)
	{
		const UserData &userData = **pos;
		SendNetworkMessage(*m_PeerInterface, userData.m_SystemAddress, payload);
	}
}

bool Server::HandleGameMessage(RakNet::Packet &a_Packet)
{
	bool success = false;
	ClientID userID;
	GameID gameID;
	ExtractMessageInfo(a_Packet, userID, gameID);
	const UserData *userData = FindUserData(m_UserData, userID);
	if (nullptr != userData)
	{
		if (userData->m_LoggedIn)
		{
			IServerGame *game = FindGame(userData->m_GameID);
			AssertMessage(nullptr != game, "Unable to retrieve game for client!");
			success = game->HandleGameMessage(a_Packet, userData->m_ClientID);
		}
		else
		{
			SendNetworkMessage(*m_PeerInterface, userData->m_SystemAddress, EServerError_NotLoggedIn);
		}
	}
	else
	{
		SendServerErrorMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_UserDataUnavailable);
		m_Logger.WriteLine(StringFormatter::Format("[HandleGameMessage] Unable to find user data for [%d]!", userID).c_str());
	}

	return success;
}

void Server::HandleServerHelp(RakNet::SystemAddress &a_SystemAddress)
{
	const std::string help = ReadFileToString(g_ServerHelpFile);
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(EMessage_RecvServerHelp));
	bitStream.Write(RakNet::RakString(help.c_str()));
	SendNetworkMessage(*m_PeerInterface, a_SystemAddress, bitStream);
}

void Server::HandleGameHelp(RakNet::SystemAddress &a_SystemAddress, EGame a_Game)
{
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(EMessage_RecvGameHelp));
	const std::string help = GetGameHelp(a_Game);
	bitStream.Write(RakNet::RakString(help.c_str()));

	SendNetworkMessage(*m_PeerInterface, a_SystemAddress, bitStream);
}

void Server::AddLobby(EGame a_Game)
{
	ILobby *lobby = FindGameLobby(m_Lobbies, a_Game);
	AssertMessage(nullptr == lobby, "Attempt to create a lobby for a game that was previously created!");
	lobby = CreateGameLobby(a_Game, m_Logger);
	m_Lobbies.push_back(lobby);
}

IServerGame* Server::FindGame(GameID a_GameID)
{
	IServerGame *game = nullptr;
	for (auto itLobby = m_Lobbies.begin(); nullptr == game && itLobby != m_Lobbies.end(); ++itLobby)
	{
		ILobby &lobby = **itLobby;
		std::vector<IServerGame*> games = lobby.GetGames();
		for (auto itGame = games.begin(); itGame != games.end(); ++itGame)
		{
			IServerGame &serverGame = **itGame;
			if (serverGame.GetGameID() == a_GameID)
			{
				game = &serverGame;
				break;
			}
		}
	}
	return game;
}

void Server::HandleDetermineCurrentPlayer(RakNet::Packet &a_Packet)
{
	ClientID clientID;
	GameID gameID;
	ExtractMessageInfo(a_Packet, clientID, gameID);
	const UserData *userData = FindUserData(m_UserData, clientID);
	if (userData->m_GameID != gameID)
	{
		SendServerErrorMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_InvalidGameID);
	}
	else
	{
		IServerGame *game = FindGame(gameID);
		AssertMessage(nullptr != game, "Unable to retrieve game for client!");
		ClientID currentPlayer = game->WhoseTurnIsIt();

		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvWhoseTurnIsIt));
		payload.Write(currentPlayer);
		SendNetworkMessage(*m_PeerInterface, a_Packet.systemAddress, payload);
	}
}

void Server::HandleSendLobbyData(RakNet::Packet& a_Packet)
{
	EGame game;
	RakNet::BitStream bitStream(a_Packet.data, a_Packet.length, false);
	bitStream.IgnoreBytes(sizeof(RakNet::MessageID));
	bitStream.Read(game);

	ILobby* lobby = FindGameLobby(m_Lobbies, game);
	if(lobby == nullptr)
	{
		SendNetworkMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_GameLobbyUnavailable);
	}
	else
	{
		RakNet::BitStream payload;
		payload.Write(static_cast<RakNet::MessageID>(EMessage_RecvLobbyData));
		payload.Write(game);
		payload.Write(static_cast<uint32_t>(lobby->GetNumPlayersPerGame()));
		payload.Write(static_cast<uint32_t>(lobby->GetWaitQueue().size()));

		std::vector<UserData*> players = lobby->GetWaitQueue();
		for (auto pos = players.begin(); pos != players.end(); ++pos)
		{
			const UserData &userData = **pos;
			payload.Write(RakNet::RakString(userData.m_Name.c_str()));
			payload.Write(static_cast<uint32_t>(userData.m_ClientID));
		}
		
		SendNetworkMessage(*m_PeerInterface, a_Packet.systemAddress, payload);
	}
}

void Server::RemovePlayer(UserData& a_UserData)
{
	if (a_UserData.m_GameID != InvalidGameID())
	{
		auto game = FindGame(a_UserData.m_GameID);

		if (nullptr == game)
		{
			RemovePlayerFromLobby(a_UserData);
		}
		else
		{
			const auto playersInGame = game->GetPlayers();
			auto *lobby = FindGameLobby(m_Lobbies, a_UserData.m_GameID);
			const GameID gameID = a_UserData.m_GameID;

			for (const auto& playerInGame : playersInGame)
			{
				SendNetworkMessage(*m_PeerInterface, playerInGame->m_SystemAddress, EMessage_RecvPlayerLeftGame);
				playerInGame->m_GameID = InvalidGameID();
				if (playerInGame->m_ClientID != a_UserData.m_ClientID)
				{
					lobby->AddToQueue(*playerInGame);
					m_Logger.WriteLine("Moved player [%s] to game lobby.", playerInGame->m_ClientID);
				}
			}
			lobby->RemoveGame(gameID);

			HandleWaitingFromPlayer(*lobby);
			m_Logger.WriteLine("Player was in the lobby for the game [%s] if any other players got a message.", a_UserData.m_GameID);
		}
	}
	else
	{
		RemovePlayerFromLobby(a_UserData);
	}

	a_UserData.m_LoggedIn = false;
	a_UserData.m_SystemAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
}

void Server::RemovePlayerFromLobby(UserData& a_UserData)
{
	ILobby* playersLobby = nullptr;
	for(auto* lobby : m_Lobbies)
	{
		if(lobby->HasWaitingUser(&a_UserData))
		{
			m_Logger.WriteLine("User was removed from lobby [%s].",lobby->GetGameType());
			playersLobby = lobby;
			lobby->RemoveFromQueue(a_UserData.m_ClientID);
			break;
		}
	}
	if(nullptr != playersLobby)
	{
		for(const auto& playerInLobby : playersLobby->GetWaitQueue())
		{
			SendNetworkMessage(*m_PeerInterface,playerInLobby->m_SystemAddress,EMessage_RecvPlayerLeftLobby);
		}
		m_Logger.WriteLine("Player was in the lobby for game [%s] other player got a message.",a_UserData.m_GameID);
		HandleWaitingFromPlayer(*playersLobby);
	}	
}