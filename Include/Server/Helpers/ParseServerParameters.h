#pragma once

#include <string>

struct ServerParameters;

void ParseServerParameters(const std::string &a_Filename, ServerParameters &a_Parameters);
