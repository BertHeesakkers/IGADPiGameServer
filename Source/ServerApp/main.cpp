#include "main.h"

#include "Framework/CommandLine.h"
#include "Framework/CommandLineParser.h"
#include "Framework/HashedString.h"
#include "Framework/Logging/FileLogger.h"
#include "Framework/Logging/ConsoleLogger.h"
#include "ServerApp.h"

#include <iostream>
#include <string>
#include <sstream>

namespace
{
	const std::string g_ParameterFile = "parameters";
	const std::string g_ParameterFileShort = "p";
	const std::string g_LogFile = "logfile";
	const std::string g_LogFileShort = "l";
	const std::string g_ErrorLogFile = "errorlogfile";
	const std::string g_ErrorLogFileShort = "e";

	void RedirectErrorOutputStream(const std::string &a_ErrorLogFilename)
	{
		std::ofstream errorLog(a_ErrorLogFilename);
		AssertMessage(errorLog.is_open(), "Failed to open error log!");
		std::cerr.rdbuf(errorLog.rdbuf());
	}
}

int main(int a_ArgC, char *a_ArgV[])
{
	CommandLineParser commandLineParser;
	commandLineParser.AddParameter(ECommandLineItemType_Optional, g_ParameterFile, g_ParameterFileShort);
	commandLineParser.AddParameter(ECommandLineItemType_Optional, g_LogFile, g_LogFileShort);
	commandLineParser.AddParameter(ECommandLineItemType_Optional, g_ErrorLogFile, g_ErrorLogFileShort);
	const CommandLine &commandLine = commandLineParser.Parse(a_ArgC, a_ArgV);

	std::string parameterfile = "data/server_parameters.json";
	if (commandLine.HasParameter(HashedString(g_ParameterFile)))
	{
		parameterfile = commandLine.GetParameter(HashedString(g_ParameterFile));
	}

	std::string logfile = "";
	bool useLogFile = false;
	if (commandLine.HasParameter(HashedString(g_LogFile)))
	{
		logfile = commandLine.GetParameter(HashedString(g_LogFile));
		useLogFile = true;
	}

	std::string errorLogfile = "server_error.log";
	if (commandLine.HasParameter(HashedString(g_ErrorLogFile)))
	{
		errorLogfile = commandLine.GetParameter(HashedString(g_ErrorLogFile));
	}
	RedirectErrorOutputStream(errorLogfile);

	ILogger *logger = nullptr;
	if (useLogFile)
	{
		logger = new FileLogger(logfile);
	}
	else
	{
		logger = new ConsoleLogger();
	}

	ServerApp app(*logger);
	app.Initialize(parameterfile);
	app.StartServer(Sync);
	delete logger;
	return 0;
}
