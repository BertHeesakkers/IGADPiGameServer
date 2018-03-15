#pragma once

#include <string>
#include <vector>

struct UserData;

void ReadUserData(const std::string &a_Filename, std::vector<UserData*> &a_UserData);
