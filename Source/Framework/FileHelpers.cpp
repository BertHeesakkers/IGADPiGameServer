#include "FileHelpers.h"

#include <assert.h>
#include <fstream>

std::string ReadFileToString(const std::string &a_Filename)
{
	std::ifstream fileStream(a_Filename.c_str());
	assert(fileStream.is_open());
	std::string fileContents;

	fileStream.seekg(0, std::ios::end);
	fileContents.reserve(static_cast<size_t>(fileStream.tellg()));
	fileStream.seekg(0, std::ios::beg);

	fileContents.assign((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());

	return fileContents;
}
