#pragma once

#include "Framework/HashedString.h"
#include "Network/ClientID.h"
#include "Network/GameID.h"
#include "Network/RakNetIncludes.h"

#include <string>
#include <vector>

struct UserData
{
	std::string m_Name;
	std::string m_ID;
	HashedString m_Passhash = HashedString::Empty;
	bool m_LoggedIn = false;
	ClientID m_ClientID = InvalidClientID();
	GameID m_GameID = InvalidGameID();
	RakNet::SystemAddress m_SystemAddress = RakNet::UNASSIGNED_SYSTEM_ADDRESS;
};

typedef std::vector<UserData*> UserDataVector;
