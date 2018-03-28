#pragma once

#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"
#include "Server/EGame.h"
#include "ILobby.h"

#include <vector>

class ILogger;
class IServerGame;
struct UserData;

class BaseLobby : public ILobby
{
public:
	BaseLobby(EGame a_Game, ILogger &a_Logger);
	~BaseLobby();

	virtual EGame GetGameType() const override;
	virtual bool CanStartNewGame() override;
	virtual void AddNewGame(GameID a_Game, RakNet::RakPeerInterface &a_PeerInterface) override;
	virtual const std::vector<IServerGame*>& GetGames() const override;
	virtual std::vector<IServerGame*>& GetGames() override;
	virtual void AddToQueue(UserData &a_UserData) override;
	/**
	* Added by Simon Renger
	* Date: 28-03-2018
	* Description: to being able to remove games see Server.cpp handel lost connection
	**/
	virtual void RemoveGame(GameID a_GameID) override;
	/**
	* Added by Simon Renger
	* Date: 27-03-2018
	* Description: needs to be here to delete a user from the queue
	* 
	**/
	virtual void RemoveFromQueue(ClientID a_ID) override;
	virtual std::vector<UserData*>& GetWaitQueue() override;
	/**
	* Added by Simon Renger
	* Date: 27-03-2018
	* Description: 
	* In order to get a user from the Queue.
	**/
	virtual bool HasWaitingUser(UserData*) override;
protected:
	virtual IServerGame* CreateNewGame(GameID a_GameID, RakNet::RakPeerInterface &a_PeerInterface) = 0;
	ILogger &m_Logger;

private:
	const EGame m_Game;
	std::vector<IServerGame*> m_Games;
	std::vector<UserData*> m_WaitQueue;
};