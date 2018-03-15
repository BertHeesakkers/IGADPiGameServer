#include "WriteUserData.h"

#include "Server/UserData.h"

#include <fstream>
#include <sstream>

void WriteUserData(const std::string &a_Filename, const std::vector<UserData*> &a_UserData)
{
	// NOTE: data file should be write-enabled; maybe force this from code?

	std::stringstream stringStream;
	stringStream << "{\"logins\":[";

	for (auto pos = a_UserData.begin(); pos != a_UserData.end(); ++pos)
	{
		const UserData &userData = **pos;
		stringStream << "{\"snr\": ";
		stringStream << userData.m_ID.c_str() << ",";
		stringStream << "\"name\": ";
		stringStream << "\"" << userData.m_Name.c_str() << "\",";
		stringStream << "\"passhash\": ";
		stringStream << "" << userData.m_Passhash.GetHash() << "}";
		if (pos != a_UserData.end() - 1)
		{
			stringStream << ",";
		}
	}
	stringStream << "]}";

	const std::string json = stringStream.str();
	std::ofstream outputFile(a_Filename);
	outputFile << json;
	outputFile.close();
}
