#include "FileHelpers.h"

#include "AssertMessage.h"
#include "StringFunctions.h"
#include "DateTimeHelpers.h"

#include <assert.h>
#include <fstream>

std::string ReadFileToString(const std::string &a_Filename)
{
	std::ifstream fileStream(a_Filename.c_str());
	AssertMessage(fileStream.is_open(), StringFormatter::Format("Unable to open [%s] for read!", a_Filename.c_str()));
	std::string fileContents;

	fileStream.seekg(0, std::ios::end);
	fileContents.reserve(static_cast<size_t>(fileStream.tellg()));
	fileStream.seekg(0, std::ios::beg);

	fileContents.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

	return fileContents;
}
