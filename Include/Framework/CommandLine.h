#pragma once

#include "AssertMessage.h"
#include "HashedString.h"
#include "LexicalCast.h"
#include "NoCopy.h"

#include <map>
#include <string>
#include <vector>

class CommandLineParser;

class CommandLine : public NoCopy
{
public:
	CommandLine();
	~CommandLine();

	bool IsValid() const;
	bool HasOptionSet(const HashedString &a_Option) const;
	bool HasParameter(const HashedString &a_Parameter) const;

	template<typename TYPE>
	const TYPE GetParameter(const HashedString &a_Parameter) const
	{
		std::map<HashedString, std::string>::const_iterator pos = m_Parameters.find(a_Parameter);
		AssertMessage(pos != m_Parameters.end(), "Parameter was not set!");
		return LexicalCast<TYPE>(pos->second);
	}
	const std::string GetParameter(const HashedString &a_Parameter) const;


private:
	void SetOption(const HashedString &a_Option);
	void SetParameter(const HashedString &a_Option, const std::string &a_Value);

	std::vector<HashedString> m_Options;
	std::map<HashedString, std::string> m_Parameters;

	bool m_IsValid;

	friend class CommandLineParser;
};
