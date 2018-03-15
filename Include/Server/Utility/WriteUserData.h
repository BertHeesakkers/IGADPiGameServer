#pragma once

#include <string>
#include <vector>

struct UserData;

void WriteUserData(const std::string &a_Filename, const std::vector<UserData*> &a_UserData);
