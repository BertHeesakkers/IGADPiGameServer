#include "CheckStartupResult.h"

#include "Framework/Logging/ILogger.h"

#include <iostream>

namespace
{
	std::string GetStartupResultMessage(RakNet::StartupResult a_Result)
	{
		std::string message;

		switch (a_Result)
		{
		case RakNet::RAKNET_STARTED:
			{
				message = "RakNet started.";
				break;
			}
		case RakNet::RAKNET_ALREADY_STARTED:
			{
				message = "RakNet already started.";
				break;
			}
		case RakNet::INVALID_SOCKET_DESCRIPTORS:
			{
				message = "Invalid socket descriptors.";
				break;
			}
		case RakNet::INVALID_MAX_CONNECTIONS:
			{
				message = "Invalid max connections.";
				break;
			}
		case RakNet::SOCKET_FAMILY_NOT_SUPPORTED:
			{
				message = "Socket familiy not supported!";
				break;
			}
		case RakNet::SOCKET_PORT_ALREADY_IN_USE:
			{
				message = "Socket port already in use.";
				break;
			}
		case RakNet::SOCKET_FAILED_TO_BIND:
			{
				message = "Socket failed to bind.";
				break;
			}
		case RakNet::SOCKET_FAILED_TEST_SEND:
			{
				message = "Socket failed test send.";
				break;
			}
		case RakNet::PORT_CANNOT_BE_ZERO:
			{
				message = "Port cannot be zero.";
				break;
			}
		case RakNet::FAILED_TO_CREATE_NETWORK_THREAD:
			{
				message = "Failed to create network thread.";
				break;
			}
		case RakNet::COULD_NOT_GENERATE_GUID:
			{
				message = "Could not generate guid.";
				break;
			}
		case RakNet::STARTUP_OTHER_FAILURE:
			{
				message = "Startup other failure.";
				break;
			}
		default:
			{
				message = "Unknown startup result.";
				break;
			}
		}
		return message;
	}
}

void CheckStartupResult(RakNet::StartupResult a_Result)
{
	std::cout << GetStartupResultMessage(a_Result).c_str() << std::endl;
}

void CheckStartupResult(RakNet::StartupResult a_Result, ILogger &a_Logger)
{
	a_Logger.WriteLine(GetStartupResultMessage(a_Result).c_str());
}