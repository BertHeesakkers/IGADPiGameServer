#pragma once

#if defined(WIN32)
#include <windows.h>
#endif

#include <boost/format.hpp>
#include <sstream>

size_t ConvertString(const char *a_Input, wchar_t *a_Output);
size_t ConvertString(const wchar_t *a_Input, char *&a_Output);
std::wstring StringToWideString(const std::string &a_String);
std::string WideStringToString(const std::wstring &a_String);
std::vector<std::string> Tokenize(const std::string &a_String, const std::string &a_Delimiters);

void LTrim(std::string &a_String);
std::string LTrim(const std::string &a_String);
void RTrim(std::string &a_String);
std::string RTrim(const std::string &a_String);
void Trim(std::string &a_String);
std::string Trim(const std::string &a_String);

template<typename TYPE>
std::string Append(const std::string &a_Text, TYPE a_Value)
{
	std::ostringstream s;
	s << a_Text << a_Value;
	return s.str();
}

class StringFormatter
{
public:
	template <typename... TArgs>
	static std::string Format(const char* a_Format, TArgs&&... a_Args)
	{
		using namespace boost::io;
		boost::format message(a_Format);
		return FormatStringRecurse(message, std::forward<TArgs>(a_Args)...);
	}

private:
	inline static std::string FormatStringRecurse(boost::format& a_Message)
	{
		return a_Message.str();
	}

	template <typename TValue, typename... TArgs>
	static std::string FormatStringRecurse(boost::format& a_Message, TValue&& a_Arg, TArgs&&... a_Args)
	{
		a_Message % std::forward<TValue>(a_Arg);
		return FormatStringRecurse(a_Message, std::forward<TArgs>(a_Args)...);
	}
};
