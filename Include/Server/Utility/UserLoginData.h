#pragma once

#include <string>

#include <cereal/archives/json.hpp>

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
