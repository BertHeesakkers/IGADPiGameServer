#include "CommandLine.h"

CommandLine::CommandLine()
: m_IsValid(false)
{
}

CommandLine::~CommandLine()
{
}

bool CommandLine::IsValid() const
{
	return m_IsValid;
}

bool CommandLine::HasOptionSet(const HashedString &a_Option) const
{
	return m_Options.end() != std::find(m_Options.begin(), m_Options.end(), a_Option);
}

bool CommandLine::HasParameter(const HashedString &a_Parameter) const
{
	return m_Parameters.end() != m_Parameters.find(a_Parameter);
}

const std::string CommandLine::GetParameter(const HashedString &a_Parameter) const
{
	std::map<HashedString, std::string>::const_iterator pos = m_Parameters.find(a_Parameter);
	AssertMessage(pos != m_Parameters.end(), ("Parameter was not set!"));
	return pos->second;
}

void CommandLine::SetOption(const HashedString &a_Option)
{
	m_Options.push_back(a_Option);
}

void CommandLine::SetParameter(const HashedString &a_Option, const std::string &a_Value)
{
	std::pair<std::map<HashedString, std::string>::iterator, bool> ret = m_Parameters.insert(std::pair<HashedString, std::string>(a_Option, a_Value));
	AssertMessage(ret.second, ("Error inserting command line parameter value!"));
}
