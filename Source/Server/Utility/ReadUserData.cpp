#include "ReadUserData.h"

#include "Framework/AssertMessage.h"
#include "Framework/FileHelpers.h"
#include "Framework/LexicalCast.h"
#include "Framework/StringFunctions.h"
#include "Server/UserData.h"

#include <cereal/archives/json.hpp>

#include <assert.h>
#include <fstream>
#include <string>

namespace
{
	struct UserLoginData
	{
		std::string m_Name;
		std::string m_ID;
		unsigned long m_Passhash;
	};

	template<class Archive>
	void serialize(Archive &a_Archive, UserLoginData &a_UserLoginData)
	{
		a_Archive(cereal::make_nvp("name", a_UserLoginData.m_Name),
			cereal::make_nvp("snr", a_UserLoginData.m_ID),
			cereal::make_nvp("passhash", a_UserLoginData.m_Passhash));
	}

}

void ReadUserData(const std::string &a_Filename, std::vector<UserData*> &a_UserData)
{
	std::ifstream fileStream(a_Filename);
	AssertMessage(fileStream.is_open(), StringFormatter::Format("Unable to open [%s] for read!", a_Filename.c_str()));
	{
		cereal::JSONInputArchive archive(fileStream);
		size_t numEntries = 0;
		archive(cereal::make_nvp("num_entries", numEntries));

		for (uint32_t entry = 0; entry < numEntries; ++entry)
		{
			UserLoginData userLoginData;
			archive(userLoginData);

			UserData *user = new UserData();
			user->m_Name = userLoginData.m_Name;
			user->m_ID = userLoginData.m_ID;
			user->m_Passhash = userLoginData.m_Passhash;
			a_UserData.push_back(user);
		}
	}
	fileStream.close();
}
