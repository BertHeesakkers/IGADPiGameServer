#include "UserDataHelpers.h"

#include "Network/RakNetIncludes.h"
#include "Framework/HashedString.h"
#include "Server/UserData.h"

#include <algorithm>

/**
* Added by Simon Renger
* Date: 27-03-2018
* Description:
* finds user data based on the system address
**/
UserData* FindUserData(const std::vector<UserData*> &a_UserData, const RakNet::SystemAddress &a_SysAddr)
{
	UserData *userData = nullptr;
	auto pos = std::find_if(a_UserData.begin(), a_UserData.end(), [&a_SysAddr](const UserData *a_UserData) { return a_UserData->m_SystemAddress == a_SysAddr; });
	if (pos != a_UserData.end())
	{
		userData = *pos;
	}
	return userData;
}

UserData* FindUserData(const std::vector<UserData*> &a_UserData, const std::string &a_ID)
{
	UserData *userData = 0;
	auto pos = std::find_if(a_UserData.begin(), a_UserData.end(), [&a_ID](const UserData *a_UserData) { return strcmp(a_UserData->m_ID.c_str(), a_ID.c_str()) == 0; });
	if (pos != a_UserData.end())
	{
		userData = *pos;
	}
	return userData;
}

UserData* FindUserData(const std::vector<UserData*> &a_UserData, const ClientID &a_ClientID)
{
	UserData *userData = 0;
	auto pos = std::find_if(a_UserData.begin(), a_UserData.end(), [&a_ClientID](const UserData *a_UserData) { return a_UserData->m_ClientID == a_ClientID; });
	if (pos != a_UserData.end())
	{
		userData = *pos;
	}
	return userData;
}

void FindUserData(const std::vector<UserData*> &a_UserData, const GameID &a_GameID, std::vector<UserData*> &a_Selection)
{
	for (auto pos = a_UserData.begin(); pos != a_UserData.end(); ++pos)
	{
		UserData &userData = **pos;
		if (userData.m_GameID == a_GameID)
		{
			a_Selection.push_back(&userData);
		}
	}
}

bool RemoveUserData(std::vector<UserData*> &a_UserData, const std::string &a_ID)
{
	bool success = false;
	auto pos = std::find_if(a_UserData.begin(), a_UserData.end(), [&a_ID](const UserData *a_UserData) { return strcmp(a_UserData->m_ID.c_str(), a_ID.c_str()) == 0; });
	if (pos != a_UserData.end())
	{
		success = true;
		a_UserData.erase(pos);
	}
	return success;
}

bool ValidateLoginData(const UserData &a_UserData, const std::string &a_ID, const HashedString &a_Passhash)
{
	return (a_UserData.m_Passhash == a_Passhash);
}
