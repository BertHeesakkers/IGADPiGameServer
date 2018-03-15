#include "NamedPipe.h"

#include "../AssertMessage.h"
#include "../GetLastErrorAsString.h"
#include "../StringFunctions.h"

#if defined(ENVIRONMENT_WINDOWS)

NamedPipe* ConstructNamedPipe(const PipeCreateParameters &a_Parameters)
{
// 	SECURITY_ATTRIBUTES securityAttributes;
// 	securityAttributes.lpSecurityDescriptor = new PSECURITY_DESCRIPTOR();
// 	if (!::InitializeSecurityDescriptor(securityAttributes.lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION))
// 	{
// 		AssertMessage("Failed to initialize security descriptor!");
// 	}
// 	if (!::SetSecurityDescriptorDacl(securityAttributes.lpSecurityDescriptor, TRUE, reinterpret_cast<PACL>(0), FALSE))
// 	{
// 		AssertMessage("Failed to set security descriptor access control!");
// 	}
// 	securityAttributes.nLength = sizeof(securityAttributes);
// 	securityAttributes.bInheritHandle = TRUE;

	const std::wstring fullPipeName = StringToWideString(std::string("\\\\.\\pipe\\" + a_Parameters.m_Name));
	HANDLE pipeHandle = ::CreateNamedPipe(fullPipeName.c_str(),
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		a_Parameters.m_OutBufferByteSize,
		a_Parameters.m_InBufferByteSize,
		NMPWAIT_USE_DEFAULT_WAIT,
		nullptr);
	AssertMessage(INVALID_HANDLE_VALUE != pipeHandle, "Failed to obtain named pipe handle!");
	return new NamedPipe(pipeHandle, a_Parameters);
}


NamedPipe::NamedPipe(HANDLE a_PipeHandle, const PipeCreateParameters &a_Parameters)
	: Pipe(a_PipeHandle, a_Parameters)
{
	AssertMessage(INVALID_HANDLE_VALUE != m_Handle, "Attempt to set invalide named pipe handle!");
}

NamedPipe::~NamedPipe()
{
	::CloseHandle(m_Handle);
}

bool NamedPipe::Connect()
{
	AssertMessage(nullptr != m_Handle, "Attempt to call connect on an uninitialized pipe handle!");
	AssertMessage(INVALID_HANDLE_VALUE != m_Handle, "Attempt to call connect on an invalid pipe handle!");
	BOOL result = ::ConnectNamedPipe(m_Handle, nullptr);
	AssertMessage(TRUE == result, "Failed to connect to named pipe!");
	return (!result || ERROR_PIPE_CONNECTED == GetLastError());
}

#endif
