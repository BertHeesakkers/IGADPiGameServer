#include "main.h"

#include "Framework/CommandLine.h"
#include "Framework/CommandLineParser.h"
#include "Framework/HashedString.h"
#include "Framework/Logging/ConsoleLogger.h"
#include "ServerApp.h"

#include <string>
#include <sstream>

#include <boost/format.hpp>

namespace
{
	const std::string g_ParameterFile = "parameters";
	const std::string g_ParameterFileShort = "p";
}

int main(int a_ArgC, char *a_ArgV[])
{
	CommandLineParser commandLineParser;
	commandLineParser.AddParameter(ECommandLineItemType_Optional, g_ParameterFile, g_ParameterFileShort);
	const CommandLine &commandLine = commandLineParser.Parse(a_ArgC, a_ArgV);

	std::string parameterfile = "data/server_parameters.json";
	if (commandLine.HasParameter(HashedString(g_ParameterFile)))
	{
		parameterfile = commandLine.GetParameter(HashedString(g_ParameterFile));
	}

	ConsoleLogger logger;
	ServerApp app(logger);
	app.Initialize(parameterfile);
	app.StartServer(Sync);
	return 0;
}

//http://www.melvilletheatre.com/articles/cstuff/3.html
//http://techlister.com/linux/creating-menu-with-ncurses-in-c/1293/
