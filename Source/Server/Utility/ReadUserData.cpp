#include "ReadUserData.h"

#include "Framework/FileHelpers.h"
#include "Framework/LexicalCast.h"
#include "Framework/StringFunctions.h"
#include "Server/ThirdParty/json.hpp"
#include "Server/UserData.h"

#include <string>

using json = nlohmann::json; 

void ParseUserData(const json &a_Json, UserData &a_UserData)
{
	for (json::const_iterator it = a_Json.begin(); it != a_Json.end(); ++it)
	{
		const std::string keyName = it.key();
		const json keyValue = it.value();

		if (keyName == std::string("name"))
		{
			a_UserData.m_Name = keyValue.get<std::string>();
		}
		if (keyName == std::string("snr"))
		{
			a_UserData.m_ID = std::to_string(keyValue.get<int>());
		}
		if (keyName == std::string("passhash"))
		{
			if (keyValue.is_number())
			{
				a_UserData.m_Passhash = static_cast<unsigned long>(keyValue.get<unsigned long>());
			}
			else
			{
				a_UserData.m_Passhash = HashedString::Empty;
			}
		}
	}
}

void ReadUserData(const std::string &a_Filename, std::vector<UserData*> &a_UserData)
{
	const std::string jsonString = ReadFileToString(a_Filename);
	const json jsonObject = json::parse(jsonString);
	
	for (json::const_iterator it = jsonObject.begin(); it != jsonObject.end(); ++it)
	{
		const std::string keyName = it.key();
		auto keyValue = it.value();

		if (keyName == std::string("logins"))
		{
			for (json::const_iterator pos = keyValue.begin(); pos != keyValue.end(); ++pos)
			{
				UserData *user = new UserData();
				ParseUserData(*pos, *user);
				a_UserData.push_back(user);
			}
		}
	}
}
