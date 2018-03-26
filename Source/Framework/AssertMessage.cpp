#include "AssertMessage.h"

#include "StringFunctions.h"

#include <assert.h>
#if __linux__
#include <iostream>
#endif
#include <string>

void AssertMessage(const std::string &a_Text)
{
	AssertMessage(false, a_Text);
}

void AssertMessage(bool a_Condition, const std::string &a_Text)
{
	if (!a_Condition)
	{
		const std::wstring text = StringToWideString(a_Text);
#ifdef WIN32
		::MessageBox(0, text.c_str(), L"Error!", MB_ICONERROR | MB_OK);
#elif __linux__
		std::cerr << "Error: " << a_Text << std::endl;
#endif
		assert(false);
	}
}
