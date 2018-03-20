#include "Server.h"

#include "Framework/AssertMessage.h"
#include "Framework/FileHelpers.h"
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
			SendMessage(a_PeerInterface, a_UserData.m_SystemAddress, payload);
		}
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
}

void Server::Start(ServerParameters &a_ServerParameters)
{
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
					SendMessage(*m_PeerInterface, packet->systemAddress, EMessage_RecvDisconnected);
					break;
				}
			case EMessage_SendWhoseTurnIsIt:
				{
					HandleDetermineCurrentPlayer(*packet);
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

void Server::GetLobbyData(std::vector<std::string> &a_Data) const
{
	(void)a_Data;
}

void Server::GetGameData(std::vector<std::string> &a_Data) const
{
	(void)a_Data;
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

void Server::GetPlayerData(std::vector<std::string> &a_Data) const
{
	(void)a_Data;
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

// const UserDataVector Server::GetUserData() const
// {
// 	UserDataVector userData;
// 	for (auto posLobby = m_Lobbies.begin(); posLobby != m_Lobbies.end(); ++posLobby)
// 	{
// 		const ILobby &lobby = **posLobby;
// 		const std::vector<IServerGame*> &games = lobby.GetGames();
// 		for (auto posGame = games.begin(); posGame != games.end(); ++posGame)
// 		{
// 			const IServerGame &game = **posGame;
// 			const UserDataVector &players = game.GetPlayers();
// 			for (auto userPos = players.begin(); userPos != players.end(); ++userPos)
// 			{
// 				UserData &data = **userPos;
// 				userData.push_back(&data);
// 			}
// 		}
// 	}
// 	return userData;
// }
// 
// const GameDataVector Server::GetGameData() const
// {
// 	GameDataVector gameData;
// 	for (auto posLobby = m_Lobbies.begin(); posLobby != m_Lobbies.end(); ++posLobby)
// 	{
// 		const ILobby &lobby = **posLobby;
// 		const std::vector<IServerGame*> &games = lobby.GetGames();
// 		for (auto posGame = games.begin(); posGame != games.end(); ++posGame)
// 		{
// 			const IServerGame &game = **posGame;
// 			GameData data;
// 			data.m_GameID = game.GetGameID();
// 		}
// 	}
// 	return gameData;
// }
// 
// const LobbyDataVector Server::GetLobbyData() const
// {
// 	LobbyDataVector lobbyData;
// 	return lobbyData;
// }

void Server::UpdateUserData()
{
	WriteUserData(g_UserDataFilename, m_UserData);
	m_UpdateUserData = false;
}

void Server::HandleLostConnection(RakNet::Packet &a_Packet)
{
	// TODO: find user data based on system address and close connection
	// remove player from game/lobbies
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
				SendMessage(*m_PeerInterface, a_Packet.systemAddress, EServerError_NoPassword);
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

	// TODO: Finish HandleLogout(...); implementation.
	// Find all players involved in the same game as the one that disconnected.
	// Cancel games and inform other players
	// Remove games

	if (a_SendMessages)
	{
		m_PeerInterface->CloseConnection(userData->m_SystemAddress, true, 0, LOW_PRIORITY);
	}
	userData->m_LoggedIn = false;
	userData->m_SystemAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
}

void Server::HandleJoinGame(EGame a_Game, UserData &a_UserData, bool a_SendMessages /* = true */)
{
	ILobby *lobby = FindGameLobby(m_Lobbies, a_Game);
	if (nullptr == lobby)
	{
		m_Logger.WriteLine("No game lobby available for game [%s].", Translate(a_Game));
		if (a_SendMessages)
		{
			SendMessage(*m_PeerInterface, a_UserData.m_SystemAddress, EServerError_GameLobbyUnavailable);
		}
	}
	else
	{
		lobby->AddToQueue(a_UserData);
		
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
					SendMessage(*m_PeerInterface, userData.m_SystemAddress, payload);
				}
			}
		}
		else
		{
			if (a_SendMessages)
			{
				SendMessage(*m_PeerInterface, a_UserData.m_SystemAddress, EMessage_RecvWaitingForPlayers);
			}
		}
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
			SendMessage(*m_PeerInterface, userData->m_SystemAddress, EServerError_NotLoggedIn);
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
	SendMessage(*m_PeerInterface, a_SystemAddress, bitStream);
}

void Server::HandleGameHelp(RakNet::SystemAddress &a_SystemAddress, EGame a_Game)
{
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(EMessage_RecvGameHelp));
	const std::string help = GetGameHelp(a_Game);
	bitStream.Write(RakNet::RakString(help.c_str()));

	SendMessage(*m_PeerInterface, a_SystemAddress, bitStream);
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
		SendMessage(*m_PeerInterface, a_Packet.systemAddress, payload);
	}
}
