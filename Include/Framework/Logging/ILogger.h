#pragma once

#include "boost/format.hpp"
#include <string>

class ILogger
{
public:
	virtual ~ILogger() { }

	template <typename... TArgs>
	void WriteLine(const char* a_Format, TArgs&&... a_Args)
	{
		using namespace boost::io;
		boost::format message(a_Format);
		WriteLine(FormatStringRecurse(message, std::forward<TArgs>(a_Args)...));
	}

private:
	virtual void WriteLine(const std::string &a_Line) = 0;

	inline static std::string FormatStringRecurse(boost::format& a_Message)
	{
		return a_Message.str();
	}

	template <typename TValue, typename... TArgs>
	std::string FormatStringRecurse(boost::format& a_Message, TValue&& a_Arg, TArgs&&... a_Args)
	{
		a_Message % std::forward<TValue>(a_Arg);
		return FormatStringRecurse(a_Message, std::forward<TArgs>(a_Args)...);
	}
};
