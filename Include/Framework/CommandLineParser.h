#pragma once

#include "NoCopy.h"

#include <map>
#include <string>

class CommandLine;
class HashedString;

enum ECommandLineItem
{
	ECommandLineItem_Parameter,
	ECommandLineItem_Option,

	ECommandLineItem_Invalid
};

enum ECommandLineItemType
{
	ECommandLineItemType_Required,
	ECommandLineItemType_Optional,
	ECommandLineItemType_Repeated,
	ECommandLineItemType_Choice,

	ECommandLineItemType_Invalid
};

struct CommandLineItem
{
	CommandLineItem(const std::string &a_ShortForm, const std::string &a_LongForm, const ECommandLineItemType a_ItemType, ECommandLineItem a_Item)
	: m_LongForm(a_LongForm)
	, m_ShortForm(a_ShortForm)
	, m_ItemType(a_ItemType)
	, m_Item(a_Item)
	{
	}

	const std::string m_LongForm;
	const std::string m_ShortForm;
	const ECommandLineItemType m_ItemType;
	const ECommandLineItem m_Item;
};

class CommandLineParser : public NoCopy
{
public:
	CommandLineParser();
	~CommandLineParser();

	void SetName(const std::string &a_Name);
	void AddOption(ECommandLineItemType a_Type, const std::string &a_LongForm, const std::string &a_ShortForm = "");
	void AddParameter(ECommandLineItemType a_Type, const std::string &a_LongForm, const std::string &a_ShortForm = "");

	const CommandLine& Parse(uint32_t a_ArgC, char *a_ArgV[]);
	const CommandLine& Parse(uint32_t a_ArgC, const char *a_ArgV[]);
	const CommandLine& Parse(const std::string &a_CommandLine);

	void OutputSyntax() const;

private:
	typedef std::map<HashedString, CommandLineItem*>	CommandLineOptions;
	typedef std::pair<HashedString, CommandLineItem*>	CommandLineOptionsType;
	typedef CommandLineOptions::iterator				CommandLineOptionsIt;
	typedef CommandLineOptions::const_iterator			CommandLineOptionsConstIt;
	typedef std::pair<CommandLineOptionsIt, bool>		CommandLineOptionsResult;

	std::string m_Name;
	CommandLineOptions m_CommandLineOptions;
	CommandLine *m_CommandLine;
};
