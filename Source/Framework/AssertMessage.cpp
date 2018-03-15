#include "AssertMessage.h"

#include "StringFunctions.h"

#include <assert.h>
#if ENVIRONMENT_PI
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
#ifdef ENVIRONMENT_WINDOWS
		::MessageBox(0, text.c_str(), L"Error!", MB_ICONERROR | MB_OK);
#elif ENVIRONMENT_PI
		std::cout << "Error: " << a_Text << std::endl;
#endif
		assert(false);
	}
}
