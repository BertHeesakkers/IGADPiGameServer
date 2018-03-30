#include "ParseServerParameters.h"

#include "Framework/AssertMessage.h"
#include "Framework/StringFunctions.h"
#include "Network/Helpers/Translate.h"
#include "Server/ServerParameters.h"

#include <cereal/archives/json.hpp>
#include <fstream>

void ParseServerParameters(const std::string &a_Filename, ServerParameters &a_Parameters) 
{
	std::ifstream fileStream(a_Filename);
	AssertMessage(fileStream.is_open(), StringFormatter::Format("Unable to open [%s] for read!", a_Filename.c_str()));

	cereal::JSONInputArchive archive(fileStream);
	archive(cereal::make_nvp("ServerPort", a_Parameters.m_ServerPort));
	archive(cereal::make_nvp("MaxClients", a_Parameters.m_MaxClients));
	std::string lobbies[4];
	archive(cereal::make_nvp("Lobbies", lobbies));
	fileStream.close();

	for (uint32_t lobby = 0; lobby < 4; lobby++)
	{
		a_Parameters.m_Lobbies.push_back(Translate(lobbies[lobby]));
	}
}