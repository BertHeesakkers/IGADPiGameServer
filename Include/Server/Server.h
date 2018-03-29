#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Server/EGame.h"
#include "GameData.h"
#include "LobbyData.h"
#include "UserData.h"

#include <string>
#include <vector>

class HashedString;
class ILobby;
class ILogger;
class IServerGame;
struct ServerParameters;

class Server
{
public:
	Server(ILogger &a_Logger, bool a_UsePacketLogger = false);
	~Server();

	void Start(ServerParameters &a_Parameters);
	void Run();
	bool IsRunning() const;
	void Stop(bool a_SendMessages = true);

	// RPC functions
	bool GetLobbyData(std::vector<std::string> &a_Data) const;
	bool GetGameData(std::vector<std::string> &a_Data) const;
	bool KillGame(const GameID &a_ID);
	bool KillAllGames();
	bool GetPlayerData(std::vector<std::string> &a_Data) const;
	bool KillPlayer(const ClientID &a_ID);
	bool KillAllPlayers();
	bool KillServer();
	bool PurgeLogFile(std::string &a_LogFilename);

private:
	void UpdateUserData();

	void HandleLostConnection(RakNet::Packet &a_Packet);
	void HandleLogin(RakNet::Packet &a_Packet, const std::string &a_ID, const HashedString &a_Passhash, bool a_SendMessages = true);
	void HandleLogout(const std::string &a_ID, bool a_SendMessages = true);
	void HandleJoinGame(EGame a_Game, UserData &a_UserData, bool a_SendMessages = true);
	bool HandleGameMessage(RakNet::Packet &a_Packet);

	void HandleServerHelp(RakNet::SystemAddress &a_SystemAddress);
	void HandleGameHelp(RakNet::SystemAddress &a_SystemAddress, EGame a_Game);

	void HandleDetermineCurrentPlayer(RakNet::Packet &a_Packet);
	void HandleSendLobbyData(RakNet::Packet &a_Packet);
	void HandleWaitingFromPlayer(ILobby &a_Lobby);

	IServerGame* FindGame(GameID a_GameID);
	void AddLobby(EGame a_Game);

	void RemovePlayer(UserData& a_UserData);
	void RemovePlayerFromLobby(UserData& a_UserData);

	ILogger &m_Logger;

	RakNet::RakPeerInterface *m_PeerInterface = nullptr;
	RakNet::PacketLogger *m_PacketLogger = nullptr;
	RakNet::RPC4 *m_RPCInterface = nullptr;

	bool m_ServerRunning = false;
	bool m_UsePacketLogger = false;
	std::vector<UserData*> m_UserData;
	std::vector<ILobby*> m_Lobbies;
	bool m_UpdateUserData = false;
};
