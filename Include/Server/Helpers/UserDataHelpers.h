#pragma once

#include "Network/ClientID.h"
#include "Network/GameID.h"

#include <string>
#include <vector>

class HashedString;
struct UserData;

UserData* FindUserData(const std::vector<UserData*> &a_UserData, const std::string &a_ID);
UserData* FindUserData(const std::vector<UserData*> &a_UserData, const ClientID &a_ClientID);
void FindUserData(const std::vector<UserData*> &a_UserData, const GameID &a_GameID, std::vector<UserData*> &a_Selection);

bool RemoveUserData(std::vector<UserData*> &a_UserData, const std::string &a_ID);
bool ValidateLoginData(const UserData &a_UserData, const std::string &a_ID, const HashedString &a_Passhash);

