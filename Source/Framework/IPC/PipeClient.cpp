#include "PipeClient.h"

#include "AnonymousPipe.h"
#include "NamedPipe.h"
#include "PipeCreateParameters.h"
#include "../AssertMessage.h"

#if defined(ENVIRONMENT_WINDOWS)

PipeClient::PipeClient()
{
}

PipeClient::~PipeClient()
{
	delete m_Pipe;
	m_Pipe = nullptr;
}

void PipeClient::Initialize(const PipeCreateParameters &a_Parameters)
{
	if (a_Parameters.m_Name.empty())
	{
	}
	else
	{
		// 	BOOL result = ::CallNamedPipe(
		// 		_In_  LPCTSTR lpNamedPipeName,
		// 		_In_  LPVOID  lpInBuffer,
		// 		_In_  DWORD   nInBufferSize,
		// 		_Out_ LPVOID  lpOutBuffer,
		// 		_In_  DWORD   nOutBufferSize,
		// 		_Out_ LPDWORD lpBytesRead,
		// 		_In_  DWORD   nTimeOut
		// 	);
	}
}

void PipeClient::Start()
{
}

#endif
