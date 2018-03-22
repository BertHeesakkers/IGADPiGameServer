#include "Pipe.h"

#include "../AssertMessage.h"
#include "../StringFunctions.h"

#if defined(WIN32)

Pipe::Pipe(HANDLE a_Handle, const PipeCreateParameters &a_Parameters)
	: m_Parameters(a_Parameters)
	, m_FullName(StringToWideString("\\\\.\\pipe\\" + a_Parameters.m_Name))
	, m_Handle(a_Handle)
{
}

bool Pipe::IsAnonymous() const
{
	return m_Parameters.m_Name.empty();
}

// void Pipe::ReadData(std::string &a_Message)
// {
// 	AssertMessage(nullptr != m_Handle, "Attempt to use an invalid pipe handle!");
// 	char buffer[1024];
// 	unsigned long bytesRead = 0;
// 	BOOL result = ::ReadFile(m_Handle, buffer, sizeof(buffer) - 1, &bytesRead, nullptr);
// 	AssertMessage(TRUE == result, "Failed to read from pipe!");
// 	buffer[bytesRead] = '\0';
// 	a_Message = std::string(buffer);
// }

HANDLE Pipe::GetHandle() const
{
	AssertMessage(nullptr != m_Handle, "Attempt to retrieve an invalid pipe handle!");
	return m_Handle;
}

void Pipe::Disconnect()
{
	AssertMessage(nullptr != m_Handle, "Attempt to use an invalid pipe handle!");
	::DisconnectNamedPipe(m_Handle);
}

uint32_t Pipe::GetInputBufferByteSize() const
{
	return m_Parameters.m_InBufferByteSize;
}

uint32_t Pipe::GetOutputBufferByteSize() const
{
	return m_Parameters.m_OutBufferByteSize;
}

#endif
