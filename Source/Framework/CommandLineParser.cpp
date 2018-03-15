#include "CommandLineParser.h"

#include "CommandLine.h"
#include "AssertMessage.h"
#include "HashedString.h"
#include "ClearContainerContents.h"

#include <iostream>
#include <sstream>

namespace
{
	const std::size_t FindParameter(const std::string &a_CommandLine, const std::string &a_ShortForm, const std::string &a_LongForm)
	{
		std::size_t pos = a_CommandLine.find("-" + a_LongForm + "=");
		if (std::string::npos == pos && !a_ShortForm.empty())
		{
			pos = a_CommandLine.find("-" + a_ShortForm + "=");
		}
		return pos;
	}

	const std::size_t FindOption(const std::string &a_CommandLine, const std::string &a_ShortForm, const std::string &a_LongForm)
	{
		std::size_t pos = a_CommandLine.find("-" + a_LongForm);
		if (std::string::npos == pos && !a_ShortForm.empty())
		{
			pos = a_CommandLine.find("-" + a_ShortForm);
		}
		return pos;
	}

	const std::size_t Find(const std::string &a_CommandLine, const std::string &a_ShortForm, const std::string &a_LongForm)
	{
		std::size_t pos = FindParameter(a_CommandLine, a_ShortForm, a_LongForm);
		if (std::string::npos == pos)
		{
			pos = FindOption(a_CommandLine, a_ShortForm, a_LongForm);
		}
		return pos;
	}
}

CommandLineParser::CommandLineParser()
	: m_CommandLine(0)
{
}

CommandLineParser::~CommandLineParser()
{
	ClearContainerContents(m_CommandLineOptions);
	delete m_CommandLine;
}

void CommandLineParser::SetName(const std::string &a_Name)
{
	m_Name = a_Name;
}

void CommandLineParser::AddOption(ECommandLineItemType a_Type, const std::string &a_LongForm, const std::string &a_ShortForm /* = "" */)
{
	CommandLineItem *item = new CommandLineItem(a_ShortForm, a_LongForm, a_Type, ECommandLineItem_Option);
	CommandLineOptionsResult ret = m_CommandLineOptions.insert(CommandLineOptionsType(HashedString(a_LongForm), item));
	AssertMessage(ret.second, "Error inserting option!");
}

void CommandLineParser::AddParameter(ECommandLineItemType a_Type, const std::string &a_LongForm, const std::string &a_ShortForm /* = "" */)
{
	CommandLineItem *item = new CommandLineItem(a_ShortForm, a_LongForm, a_Type, ECommandLineItem_Parameter);
	CommandLineOptionsResult ret = m_CommandLineOptions.insert(CommandLineOptionsType(HashedString(a_LongForm), item));
	AssertMessage(ret.second, "Error inserting option!");
}

const CommandLine& CommandLineParser::Parse(uint32_t a_ArgC, char *a_ArgV[])
{
	std::stringstream stringStream;
	for (uint32_t i = 0; i < a_ArgC; ++i)
	{
		stringStream << a_ArgV[i] << " ";
	}
	return Parse(stringStream.str());
}

const CommandLine& CommandLineParser::Parse(uint32_t a_ArgC, const char *a_ArgV[])
{
	std::stringstream stringStream;
	for (uint32_t i = 0; i < a_ArgC; ++i)
	{
		stringStream << a_ArgV[i] << " ";
	}
	return Parse(stringStream.str());
}

const CommandLine& CommandLineParser::Parse(const std::string &a_CommandLine)
{
	m_CommandLine = new CommandLine();

	for (CommandLineOptionsConstIt posItem = m_CommandLineOptions.begin(); posItem != m_CommandLineOptions.end(); ++posItem)
	{
		CommandLineItem &item = *(posItem->second);
		std::size_t pos = Find(a_CommandLine, item.m_ShortForm, item.m_LongForm);
		if (std::string::npos != pos)
		{
			switch (item.m_Item)
			{
			case ECommandLineItem_Option:
			{
				m_CommandLine->SetOption(item.m_LongForm);
				break;
			}
			case ECommandLineItem_Parameter:
			{
				pos = a_CommandLine.find_first_of("=", pos);
				std::size_t endPos = a_CommandLine.find_first_of(" ", pos);
				std::size_t length = std::string::npos;
				if (std::string::npos != endPos)
				{
					length = endPos - pos - 1;
				}

				std::string value = a_CommandLine.substr(pos + 1, length);
				m_CommandLine->SetParameter(item.m_LongForm, value);
				break;
			}
			default:
			{
				assert(false);
				break;
			}
			}
		}
		else
		{
			switch (item.m_ItemType)
			{
			case ECommandLineItemType_Required:
			{
				assert(false);
				break;
			}
			case ECommandLineItemType_Optional:
			case ECommandLineItemType_Repeated:
			case ECommandLineItemType_Choice:
			default:
				break;
			}
		}
	}
	m_CommandLine->m_IsValid = true;

	return *m_CommandLine;
}

void CommandLineParser::OutputSyntax() const
{
	std::stringstream syntax;
	syntax << "Usage: " << m_Name << " ";

	for (CommandLineOptionsConstIt pos = m_CommandLineOptions.begin(); pos != m_CommandLineOptions.end(); ++pos)
	{
		const CommandLineItem &item = *(pos->second);
		switch (item.m_ItemType)
		{
		case ECommandLineItemType_Optional:
		{
			syntax << "[-" << item.m_LongForm << "] ";
			break;
		}
		case ECommandLineItemType_Required:
		{
			syntax << "<" << item.m_LongForm << "> ";
			break;
		}
		case ECommandLineItemType_Repeated:
		{
			syntax << "<" << item.m_LongForm << "1> " << "[" << item.m_LongForm << "2] ";
			break;
		}
		case ECommandLineItemType_Choice:
		case ECommandLineItemType_Invalid:
		default:
		{
			assert(false);
			break;
		}
		}
	}

	std::cout << syntax.str() << std::endl;
}
