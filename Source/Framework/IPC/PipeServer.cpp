#include "PipeServer.h"

#include "AnonymousPipe.h"
#include "NamedPipe.h"
#include "PipeCreateParameters.h"
#include "../AssertMessage.h"

#if defined(ENVIRONMENT_WINDOWS)

PipeServer::PipeServer()
{
}

PipeServer::~PipeServer()
{
	m_ReadThread.join();
	delete m_Pipe;
}

void PipeServer::Initialize(const PipeCreateParameters &a_Parameters)
{
	if (!a_Parameters.m_Name.empty())
	{
		m_Pipe = ConstructNamedPipe(a_Parameters);
	}
	else
	{
		//m_Pipe = CreateAnonymousPipe(a_Parameters);
	}
	AssertMessage(nullptr != m_Pipe, "Failed to initialize server pipe!");
}

std::future<std::string> PipeServer::ReadData()
{
	AssertMessage(nullptr != m_Pipe, "Attempt to run without a valid pipe!");
	std::promise<std::string> promise;
	std::future<std::string> future = promise.get_future();
	m_ReadThread = std::thread(WaitForPipeInput, m_Pipe, std::move(promise));
	return future;
}

void PipeServer::WaitForPipeInput(Pipe *a_Pipe, std::promise<std::string> &&a_Result)
{
	AssertMessage(nullptr != a_Pipe, "Attempt to use an invalid pipe!");

	char *buffer = new char[a_Pipe->GetOutputBufferByteSize()];
	BOOL result = ::ConnectNamedPipe(a_Pipe->GetHandle(), 0);
	AssertMessage(result || ERROR_PIPE_CONNECTED == GetLastError(), "Error connecting to pipe for read!");

	DWORD bytesRead = 0;
	if (!(::ReadFile(a_Pipe->GetHandle(), buffer, a_Pipe->GetOutputBufferByteSize(), &bytesRead, 0)))
	{
		AssertMessage("Error reading data from named pipe!");
	}
	a_Pipe->Disconnect();

	buffer[bytesRead] = '\0';
	a_Result.set_value(std::string(&buffer[0]));
	delete[] buffer;
}

#endif
