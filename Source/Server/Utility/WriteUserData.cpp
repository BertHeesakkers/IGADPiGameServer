#include "WriteUserData.h"

#include "Framework/AssertMessage.h"
#include "Framework/StringFunctions.h"
#include "Server/UserData.h"
#include "Server/Utility/UserLoginData.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/vector.hpp>

#include <fstream>
#include <sstream>

void WriteUserData(const std::string &a_Filename, const std::vector<UserData*> &a_UserData)
{
	std::ofstream fileStream(a_Filename);
	AssertMessage(fileStream.is_open(), StringFormatter::Format("Unable to open [%s] for read!", a_Filename.c_str()));

	cereal::JSONOutputArchive archive(fileStream);
	archive(cereal::make_nvp("num_entries", a_UserData.size()));

	std::vector<UserLoginData> userLoginData;
	for (auto const &userData : a_UserData)
	{
		UserLoginData loginData;
		loginData.m_Name = userData->m_Name;
		loginData.m_ID = userData->m_ID;
		loginData.m_Passhash = userData->m_Passhash;
		userLoginData.push_back(loginData);
	}
	archive(userLoginData);
}