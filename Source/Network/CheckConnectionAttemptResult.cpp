#include "CheckConnectionAttemptResult.h"

#include <iostream>

void CheckConnectionAttemptResult(RakNet::ConnectionAttemptResult a_Result)
{
	switch (a_Result)
	{
	case RakNet::CONNECTION_ATTEMPT_STARTED:
		{
			std::cout << "Connection attempt started." << std::endl;
			break;
		}
	case RakNet::INVALID_PARAMETER:
		{
			std::cout << "Invalid parameter." << std::endl;
			break;
		}
	case RakNet::CANNOT_RESOLVE_DOMAIN_NAME:
		{
			std::cout << "Cannot resolve domain name." << std::endl;
			break;
		}
	case RakNet::ALREADY_CONNECTED_TO_ENDPOINT:
		{
			std::cout << "Already connected to endpoint." << std::endl;
			break;
		}
	case RakNet::CONNECTION_ATTEMPT_ALREADY_IN_PROGRESS:
		{
			std::cout << "Connection attempt already in progress." << std::endl;
			break;
		}
	case RakNet::SECURITY_INITIALIZATION_FAILED:
		{
			std::cout << "Security initialization failed." << std::endl;
			break;
		}
	default:
		{
			std::cout << "Unknown connection attempt result." << std::endl;
			break;
		}
	}
}