#include "ServerApp.h"

#include "Framework/AssertMessage.h"
#include "Framework/Logging/ILogger.h"
#include "Server/Helpers/ParseServerParameters.h"

ServerApp::ServerApp(ILogger &a_Logger)
	: m_Server(a_Logger)
	, m_Logger(a_Logger)
{
}

ServerApp::~ServerApp()
{
}

void ServerApp::Initialize(const std::string &a_ParameterFile)
{
	ParseServerParameters(a_ParameterFile, std::ref(m_Parameters));
}

void ServerApp::StartServer(ESync a_Synchronousity)
{
	switch (a_Synchronousity)
	{
	case Async:
		{
			if (!m_Server.IsRunning())
			{
				m_ServerThread = std::thread(&ServerApp::RunServer, this);
			}
			else
			{
				m_Logger.WriteLine("Server was already running. Unable to start.");
			}
			break;
		}
	case Sync:
		{
			ServerApp::RunServer();
			break;
		}
	default:
		{
			AssertMessage("Invalid option encountered!");
			break;
		}
	}
}

int ServerApp::GetStatusCode() const
{
	return m_StatusCode;
}

void ServerApp::StopServer()
{
	if (m_Server.IsRunning())
	{
		m_Server.Stop(true);
		m_ServerThread.join();
	}
	else
	{
		m_Logger.WriteLine("Server was not running. Unable to stop.");
	}
}

void ServerApp::RunServer()
{
	m_Server.Start(m_Parameters);
	m_Server.Run();
}