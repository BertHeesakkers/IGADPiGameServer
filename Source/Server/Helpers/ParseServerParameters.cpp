#include "ParseServerParameters.h"

#include "Framework/FileHelpers.h"
#include "Framework/StringFunctions.h"
#include "Network/Helpers/Translate.h"
#include "Server/EGame.h"
#include "Server/ThirdParty/json.hpp"
#include "Server/ServerParameters.h"

#include <assert.h>

void ParseServerParameters(const std::string &a_Filename, ServerParameters &a_Parameters) 
{
	using json = nlohmann::json;

	const std::string jsonString = ReadFileToString(a_Filename);
	const json jsonObject = json::parse(jsonString);

	for (json::const_iterator it = jsonObject.begin(); it != jsonObject.end(); ++it) 
	{
		const std::string keyName = it.key();
		auto keyValue = it.value();

		if (keyName == std::string("ServerPort"))
		{
			a_Parameters.m_ServerPort = static_cast<unsigned short>(keyValue.get<unsigned short>());
		}
		else if (keyName == std::string("MaxClients"))
		{
			a_Parameters.m_MaxClients = static_cast<unsigned short>(keyValue.get<unsigned short>());
		}
		else if (keyName == std::string("Lobbies"))
		{
			for (json::const_iterator pos = keyValue.begin(); pos != keyValue.end(); ++pos)
			{
				const std::string lobbyName = pos.value();
				const EGame game = Translate(lobbyName);
				a_Parameters.m_Lobbies.push_back(game);
			}
		}
	}
}
