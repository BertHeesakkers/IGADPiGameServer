#include "ReadUserData.h"

#include "Framework/AssertMessage.h"
#include "Framework/FileHelpers.h"
#include "Framework/LexicalCast.h"
#include "Framework/StringFunctions.h"
#include "Server/UserData.h"
#include "Server/Utility/UserLoginData.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <assert.h>
#include <fstream>
#include <string>

void ReadUserData(const std::string &a_Filename, std::vector<UserData*> &a_UserData)
{
	std::ifstream fileStream(a_Filename);
	AssertMessage(fileStream.is_open(), StringFormatter::Format("Unable to open [%s] for read!", a_Filename.c_str()));

	cereal::JSONInputArchive archive(fileStream);
	size_t numEntries = 0;
	archive(cereal::make_nvp("num_entries", numEntries));

	std::vector<UserLoginData> userLoginData;
	archive(userLoginData);
	AssertMessage(userLoginData.size() == numEntries, StringFormatter::Format("Unable to open [%s] for read!", a_Filename.c_str()));
	for (auto const &loginData : userLoginData)
	{
		UserData *userData = new UserData();
		userData->m_Name = loginData.m_Name;
		userData->m_ID = loginData.m_ID;
		userData->m_Passhash = loginData.m_Passhash;
		a_UserData.push_back(userData);
	}

	fileStream.close();
}
