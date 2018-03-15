#include "StringFunctions.h"

#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

size_t ConvertString(const char *a_Input, wchar_t *a_Output)
{
#if defined(ENVIRONMENT_WINDOWS)
	const size_t originalSize = strlen(a_Input) + 1;
	const size_t newSize = 256;
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, a_Output, originalSize, a_Input, _TRUNCATE);
	return convertedChars;
#elif defined(ENVIRONMENT_PI)
	return 0;
#endif
}

size_t ConvertString(const wchar_t *a_Input, char *&a_Output)
{
#if defined(ENVIRONMENT_WINDOWS)
	const size_t originalSize = wcslen(a_Input);
	const int newSize = WideCharToMultiByte(CP_UTF8, 0, &a_Input[0], static_cast<int>(originalSize), 0, 0, 0, 0);
	a_Output = new char[newSize];
	WideCharToMultiByte(CP_UTF8, 0, &a_Input[0], static_cast<int>(originalSize), &a_Output[0], newSize, 0, 0);
	a_Output[newSize] = '\0';
	return newSize;
#elif defined(ENVIRONMENT_PI)
	return 0;
#endif
}

std::wstring StringToWideString(const std::string &a_String)
{
#if defined(ENVIRONMENT_WINDOWS)
	const int originalLength = static_cast<int>(a_String.length()) + 1;
	int length = MultiByteToWideChar(CP_ACP, 0, a_String.c_str(), originalLength, 0, 0);
	std::wstring convertedString(length, L'\0');
	MultiByteToWideChar(CP_ACP, 0, a_String.c_str(), originalLength, &convertedString[0], length);
	return convertedString;
#elif defined(ENVIRONMENT_PI)
	return std::wstring();
#endif
}

std::string WideStringToString(const std::wstring &a_String)
{
#if defined(ENVIRONMENT_WINDOWS)
	const int originalLength = static_cast<int>(a_String.length()) + 1;
	int length = WideCharToMultiByte(CP_ACP, 0, a_String.c_str(), originalLength, 0, 0, 0, 0);
	std::string convertedString(length, '\0');
	WideCharToMultiByte(CP_ACP, 0, a_String.c_str(), originalLength, &convertedString[0], length, 0, 0);
	return convertedString;
#elif defined(ENVIRONMENT_PI)
	return std::string();
#endif
}

std::vector<std::string> Tokenize(const std::string &a_String, const std::string &a_Delimiters)
{
#if defined(ENVIRONMENT_WINDOWS)
	std::vector<std::string> tokens;
	std::string::size_type lastPos = a_String.find_first_not_of(a_Delimiters, 0);
	std::string::size_type pos = a_String.find_first_of(a_Delimiters, lastPos);
	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		tokens.push_back(a_String.substr(lastPos, pos - lastPos));
		Trim(tokens[tokens.size() - 1]);
		lastPos = a_String.find_first_not_of(a_Delimiters, pos);
		pos = a_String.find_first_of(a_Delimiters, lastPos);
	}
	return tokens;
#elif defined(ENVIRONMENT_PI)
	return std::vector<std::string>();
#endif
}

void LTrim(std::string &a_String)
{
	a_String.erase(a_String.begin(), std::find_if(a_String.begin(), a_String.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
}

void RTrim(std::string &a_String) 
{
	a_String.erase(std::find_if(a_String.rbegin(), a_String.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), a_String.end());
}

void Trim(std::string &a_String)
{
	LTrim(a_String);
	RTrim(a_String);
}

std::string LTrim(const std::string &a_String)
{
	std::string copy(a_String);
	LTrim(copy);
	return copy;
}

std::string RTrim(const std::string &a_String)
{
	std::string copy(a_String);
	RTrim(copy);
	return copy;
}

std::string Trim(const std::string &a_String)
{
	std::string copy(a_String);
	LTrim(copy);
	RTrim(copy);
	return copy;
}