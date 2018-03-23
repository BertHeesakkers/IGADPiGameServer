#include "WriteToNamedPipe.h"

#include "../AssertMessage.h"
#include "../GetLastErrorAsString.h"
#include "../StringFunctions.h"

#if defined(WIN32)

#include <windows.h>

void WriteToNamedPipe(const std::string &a_PipeName, const std::string &a_Data)
{
	const std::wstring fullPipeName = StringToWideString("\\\\.\\pipe\\" + a_PipeName);
	HANDLE pipeHandle = INVALID_HANDLE_VALUE;
	while (true)
	{
		pipeHandle = ::CreateFile(&fullPipeName.c_str()[0],
			GENERIC_WRITE, 0, nullptr,
			OPEN_EXISTING, 0, 0);
		if (INVALID_HANDLE_VALUE == pipeHandle)
		{
			if (ERROR_PIPE_BUSY != GetLastError())
			{
				AssertMessage(GetLastErrorAsString().c_str());
			}

			const DWORD timeOutMilliSeconds = 20000;
			if (!::WaitNamedPipe(&fullPipeName.c_str()[0], timeOutMilliSeconds))
			{
				AssertMessage("Error while waiting for named pipe!");
			}
		}
		else
		{
			break;
		}
	}

	DWORD bytesRead = 0;
	const DWORD bytesToWrite = static_cast<DWORD>(a_Data.size());
	if (!(::WriteFile(pipeHandle, reinterpret_cast<const void*>(a_Data.c_str()), bytesToWrite, &bytesRead, 0)))
	{
		::CloseHandle(pipeHandle);
		AssertMessage("Failed to write data to named pipe!");
	}
	::CloseHandle(pipeHandle);
}

#elif defined(__linux__)

void WriteToNamedPipe(const std::string &a_PipeName, const std::string &a_Data)
{
}

#endif
