#pragma once

#include "Framework/NoCopy.h"
#include "Server/Server.h"
#include "Server/ServerParameters.h"

#include <string>
#include <thread>

class ILogger;

struct UpdateEvent
{
	float m_DeltaTime;
};

enum ESync
{
	Async,
	Sync
};

class ServerApp : public NoCopy
{
public:
	static const int STATUSCODE_SUCCESS = 1;

public:
	ServerApp(ILogger &a_Logger);
	virtual ~ServerApp();

	void Initialize(const std::string &a_ParameterFile);
	void StartServer(ESync a_Synchronousity);
	int GetStatusCode() const;
	void StopServer();

private:
	void RunServer();

	std::thread m_ServerThread;
	Server m_Server;
	ILogger &m_Logger;
	ServerParameters m_Parameters;
	int m_StatusCode = STATUSCODE_SUCCESS;
};
